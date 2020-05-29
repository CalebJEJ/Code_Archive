#lang racket

(require parser-tools/lex parser-tools/yacc "w1c-ast.rkt")

(provide parse)

; See https://docs.racket-lang.org/parser-tools/index.html
; for details about lexers and parsers in Scheme
; (lex and yacc style)

; === The W1C Lexer ===

; Definition of tokens

; value-tokens will carry values:
; variable names and integer respectively
(define-tokens value-tokens (ID NUMBER))

(define-empty-tokens keyword-tokens
  (SKIP ASSIGN SEQ IF THEN ELSE END WHILE DO ADD SUB MUL AND LT EQ NOT LPAR RPAR TRUE FALSE INPUT PRINT REPEAT UNTIL EOF)) 

; Abbreviations for some regular expressions
(define-lex-abbrevs
  (lower-letter (char-range #\a #\z))
  (upper-letter (char-range #\A #\Z))
  (digit (char-range #\0 #\9))
  (keyword (union "skip" ":=" ";" "if" "then" "else" "end"
                  "while" "do" "+" "-" "*" "&&" "<" "==" "!"
                  "(" ")" "true" "false" "input" "print"
                  "repeat" "until")))

(define-lex-abbrevs
  (letter (union lower-letter upper-letter)))

(define-lex-abbrevs
  (identifier (concatenation letter (repetition 0 +inf.0 (union letter digit)))))

(define-lex-abbrevs
  (integer (concatenation (repetition 0 1 #\-) (repetition 1 +inf.0 digit))))


; For each keyword of W1C associates the string for this keyword to the symbol token representing it
(define lexeme-keyword-database
  '( ("skip" . SKIP) (":=" . ASSIGN) (";" . SEQ) ("if" . IF) ("then" . THEN)
                     ("else" . ELSE) ("end" . END) ("while" . WHILE) ("do" . DO)
                     ("+" . ADD) ("-" . SUB) ("*" . MUL) ("&&" . AND) ("!" . NOT)
                     ("==" . EQ) ("<" . LT) ("(" . LPAR) ( ")" . RPAR ) ("true" . TRUE)
                     ("false" . FALSE) ("input" . INPUT) ("print" . PRINT)
                     ("repeat" . REPEAT) ("until" . UNTIL) )
  )

; Convertion from a string keyword to a token keyword
(define (lexeme->keyword lexeme)
  (cdr (assoc lexeme lexeme-keyword-database)))
    
; The lexer for W1C syntax:
(define w1c-lexer
  (lexer-src-pos
   [ keyword    (lexeme->keyword lexeme) ]
   [ identifier (token-ID (string->symbol lexeme)) ]
   [ integer    (token-NUMBER (string->number lexeme))]
   [ (eof)      'EOF ]
   [ whitespace (return-without-pos (w1c-lexer input-port))]
  ))

; === The W1C Parser ===

; Helper function to display a position
(define (display-pos pos)
  (begin 
    (display "[")(display (position-line pos))
    (display ":")(display (position-col pos))(display "]")))

; Function when a syntax error occurs
(define error-management
  (lambda (tok-ok? tok-name tok-value start-pos end-pos)
    (begin
      (display "Syntax error between ")
      (display-pos start-pos)(display " and ")
      (display-pos end-pos)(displayln ""))))

; The parser that builds an AST
(define w1c-parser
  (parser
   (src-pos)
   
   (start com)
   (end EOF)
   (tokens value-tokens keyword-tokens)
   
   (error error-management)
   
   (precs (right EQ)
          (right LT)
          (left SUB ADD)
          (left MUL)
          (left NOT)
          (left AND)
          (left SEQ))
   
   (grammar

    (exp [(NUMBER) $1]
         [(ID) $1]
         [(TRUE) 'true]
         [(FALSE) 'false]
         [(exp AND exp) (list 'and $1 $3)]
         [(exp ADD exp) (list 'add $1 $3)]
         [(exp SUB exp) (list 'sub $1 $3)]
         [(exp MUL exp) (list 'mul  $1 $3)]
         [(exp LT exp)  (list 'lt $1 $3)]
         [(exp EQ exp)  (list 'eq $1 $3)]
         [(NOT exp)     (list 'not $2)]
         [(LPAR exp RPAR) $2])

    (com [(SKIP) 'skip]
         [(ID ASSIGN exp) (mk-assign $1 $3)]
         [(com SEQ com) (mk-seq $1 $3)]
         [(IF exp THEN com ELSE com END) (mk-if $2 $4 $6)]
         [(WHILE exp DO com END) (mk-while $2 $4)]
         [(PRINT ID)(mk-print $2)]
         [(INPUT ID)(mk-input $2)]
         [(REPEAT com UNTIL exp) (mk-repeat $2 $4)]
         )

    )))

; The syntax analysis function
(define (parse ip)
  (port-count-lines! ip)
  (w1c-parser (lambda ()(w1c-lexer ip))))