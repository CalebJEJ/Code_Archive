:- initialization main.

sum([], 0).
sum([H|T], Sum) :- number(H), sum(T, S), Sum is S+H.

main(_) :-
    write('== Assignment 08 ==\n'),
    consult('hw08.prolog'),
    (catch(name(Name), E, (print_message(error, E),fail))) ->
	 (write('Student: '),write(Name),write('\n\n'),


	  write('TEST: find/3\n  '),
	  (catch((find(x,V1,[(x,1)]), V1=1),
		 E, (print_message(error, E),fail)) ->
	       (write('find(x,V,[(x,1)]) succeeds with V=1: 1 point\n'), L1=1);
	   (write('find(x,V,[(x,1)]) fails: 0 point\n'), L1=0)),

	  	  write('TEST: find/3\n  '),
	  (catch((\+find(x,V2,[])),
		 E, (print_message(error, E),fail)) ->
	       (write('find(x,V,[]) fails as expected: 1 point\n'), L2=1);
	   (write('find(x,V,[]) does not fail as expected: 0 point\n'), L2=0)),

	  	  write('TEST: find/3\n  '),
	  (catch((find(x,V3,[(x,1),(y,2),(z,3)]), V3=1),
		 E, (print_message(error, E),fail)) ->
	       (write('find(x,V,[(x,1),(y,2),(z,3)]) succeeds with V=1: 1 point\n'), L3=1);
	   (write('find(x,V,[(x,1),(y,2),(z,3)]) fails: 0 point\n'), L3=0)),

	  	  	  write('TEST: find/3\n  '),
	  (catch((find(y,V4,[(x,1),(y,2),(z,3)]), V4=2),
		 E, (print_message(error, E),fail)) ->
	       (write('find(y,V,[(x,1),(y,2),(z,3)]) succeeds with V=2: 1 point\n'), L4=1);
	   (write('find(y,V,[(x,1),(y,2),(z,3)]) fails: 0 point\n'), L4=0)),

	  	  	  write('TEST: find/3\n  '),
	  (catch((find(z,V5,[(x,1),(y,2),(a,4),(b,5),(z,3)]), V5=3),
		 E, (print_message(error, E),fail)) ->
	       (write('find(z,V,[(x,1),(y,2),(a,4),(b,5),(z,3)]) succeeds with V=3: 1 point\n'), L5=1);
	   (write('find(x,V,[(x,1),(y,2),(a,4),(b,5),(z,3)]) fails: 0 point\n'), L5=0)),

	  	  	  write('TEST: find/3\n  '),
	  (catch((find(V6,2,[(x,1),(y,2),(z,3)]), V6=y),
		 E, (print_message(error, E),fail)) ->
	       (write('find(V,2,[(x,1),(y,2),(z,3)]) succeeds with V=y: 1 point\n'), L6=1);
	   (write('find(V,2,[(x,1),(y,2),(z,3)]) fails: 0 point\n'), L6=0)),


	  List1 = [push(2),push(4),mul,fetch(x),add,push(100),lt],
	  List2 = [push(2),push(4),mul,fetch(x),add,push(100),eq,push(2),push(4),mul,fetch(x),add,push(100),lt],
	  
	  write('TEST: true/false\n  '),
	  (catch(am([true | List1],[10,32],[(x,10)],List1,[true,10,32],[(x,10)]), E, (print_message(error, E),fail)) ->
	      (write('am([true,push(2),push(4),mul,fetch(x),add,push(100),lt], [10,32], [(x,10)],   [push(2),push(4),mul,fetch(x),add,push(100),lt], [true,10,32], [(x,10)]) succeeds: 1 point\n'), L7=1);
	   (write('am([true,push(2),push(4),mul,fetch(x),add,push(100),lt], [10,32], [(x,10)],   [push(2),push(4),mul,fetch(x),add,push(100),lt], [true,10,32], [(x,10)]) fails: 0 point\n'), L7=0)),

	  write('TEST: true/false\n  '),
	  (catch(am([false | List1],[10,32],[(x,10)],List1,[false,10,32],[(x,10)]), E, (print_message(error, E),fail)) ->
	      (write('am([false,push(2),push(4),mul,fetch(x),add,push(100),lt], [10,32], [(x,10)],   [push(2),push(4),mul,fetch(x),add,push(100),lt], [false,10,32], [(x,10)]) succeeds: 1 point\n'), L8=1);
	   (write('am([false,push(2),push(4),mul,fetch(x),add,push(100),lt], [10,32], [(x,10)],   [push(2),push(4),mul,fetch(x),add,push(100),lt], [false,10,32], [(x,10)]) fails: 0 point\n'), L8=0)),


	  write('TEST: true/false\n  '),
	  (catch(am([true],[],[(z,1)],[],[true],[(z,1)]), E, (print_message(error, E),fail)) ->
	      (write('am([true], [], [(z,1)],    [], [true], [(z,1)]) succeeds: 1 point\n'), L9=1);
	   (write('am([true], [], [(z,1)],    [], [true], [(z,1)]) fails: 0 point\n'), L9=0)),


	  write('TEST: true/false\n  '),
	  (catch(am([false],[1],[(t,12)],[],[false,1],[(t,12)]), E, (print_message(error, E),fail)) ->
	      (write('am([false], [1], [(t,12)],   [], [false,1], [(t,12)]) succeeds: 1 point\n'), L10=1);
	   (write('am([false], [1], [(t,12)],   [], [false,1], [(t,12)]) fails: 0 point\n'), L10=0)),

	  write('TEST: not\n  '),
	  (catch(am([not,false],[true,1],[(t,12)],[false],[false,1],[(t,12)]), E, (print_message(error, E),fail)) ->
	      (write('am([not,false], [true,1], [(t,12)],   [false], [false,1], [(t,12)]) succeeds: 1 point\n'), L11=1);
	   (write('am([not,false], [true,1], [(t,12)],   [false], [false,1], [(t,12)]) fails: 0 point\n'), L11=0)),

	  write('TEST: not\n  '),
	  (catch(am([not,false],[false,2],[(t,12)],[false],[true,2],[(t,12)]), E, (print_message(error, E),fail)) ->
	      (write('am([not,false], [false,2],[(t,12)],   [false], [true,2], [(t,12)]) succeeds: 1 point\n'), L12=1);
	   (write('am([not,false], [false,2],[(t,12)],   [false], [true,2], [(t,12)]) fails: 0 point\n'), L12=0)),


	  write('TEST: and\n  '),
	  (catch(am([and,false],[true,false],[(t,12)],[false],[false],[(t,12)]), E, (print_message(error, E),fail)) ->
	      (write('am([and,false], [true,false], [(t,12)],   [false], [false], [(t,12)]) succeeds: 1 point\n'), L13=1);
	   (write('am([and,false], [true,false], [(t,12)],   [false], [false], [(t,12)]) fails: 0 point\n'), L13=0)),


	  write('TEST: and\n  '),
	  (catch(am([and,false],[true,true],[(t,12)],[false],[true],[(t,12)]), E, (print_message(error, E),fail)) ->
	      (write('am([and,false], [true,true], [(t,12)],   [false], [true], [(t,12)]) succeeds: 1 point\n'), L14=1);
	   (write('am([and,false], [true,true], [(t,12)],   [false], [true], [(t,12)]) fails: 0 point\n'), L14=0)),


	  write('TEST: and\n  '),
	  (catch(am([and,false],[false,false],[(t,12)],[false],[false],[(t,12)]), E, (print_message(error, E),fail)) ->
	      (write('am([and,false], [false,false], [(t,12)],   [false], [false], [(t,12)]) succeeds: 1 point\n'), L15=1);
	   (write('am([and,false], [false,false], [(t,12)],   [false], [false], [(t,12)]) fails: 0 point\n'), L15=0)),


	  write('TEST: and\n  '),
	  (catch(am([and,false],[false,true],[(t,12)],[false],[false],[(t,12)]), E, (print_message(error, E),fail)) ->
	      (write('am([and,false], [false,true], [(t,12)],   [false], [false], [(t,12)]) succeeds: 1 point\n'), L16=1);
	   (write('am([and,false], [false,true], [(t,12)],   [false], [false], [(t,12)]) fails: 0 point\n'), L16=0)),
	  
	  
	  write('TEST: lt\n  '),
	  (catch(am([lt], [1,2],[],[],[true],[]), E, (print_message(error, E),fail)) ->
	      (write('am([lt], [1,2], [],   [], [true], []) succeeds: 1 point\n'), L17=1);
	   (write('am([lt], [1,2], [],   [], [true], []) fails: 0 point\n'), L17=0)),

	  write('TEST: lt\n  '),
	  (catch(am([lt], [2,2,1],[],[],[false,1],[]), E, (print_message(error, E),fail)) ->
	       (write('am([lt], [2,2,1], [],   [], [false,1], []) succeeds: 1 point\n'), L18=1);
	   (write('am([lt], [2,2,1], [],   [], [false,1], []) fails: 0 point\n'), L18=0)),

	  write('TEST: eq\n  '),
	  (catch(am([eq], [1,2],[],[],[false],[]), E, (print_message(error, E),fail)) ->
	      (write('am([eq], [1,2], [],   [], [false], []) succeeds: 1 point\n'), L19=1);
	   (write('am([eq], [1,2], [],   [], [false], []) fails: 0 point\n'), L19=0)),

	  write('TEST: eq\n  '),
	  (catch(am([eq], [2,2,1],[],[],[true,1],[]), E, (print_message(error, E),fail)) ->
	       (write('am([lt], [2,2,1], [],   [], [true,1], []) succeeds: 1 point\n'), L20=1);
	   (write('am([eq], [2,2,1], [],   [], [true,1], []) fails: 0 point\n'), L20=0)),
	  
	  
	  Scores = [L1,L2,L3,L4,L5,L6,L7,L8,L9,L10,L11,L12,L13,L14,L15,L16,L17,L18,L19,L20],
	  sum(Scores, Score),
	  write('\nSCORE = '),write(Score),write('\n'));
    write('No name defined\n').
