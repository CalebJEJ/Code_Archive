public class test{

   public static void main(String[] args)
   {
	   Purchase testPurchase = new Purchase(3);
	   
	   Shelf testShelf = new Shelf();
	   Storage testStorage = new Storage();
	   
	   
	   Special testProduct = new Special(testShelf, 10, 10);
	   
	   
	   testPurchase.addProduct(testProduct);
	   
	   System.out.println(testPurchase.getTotal() + " Expected 110");
	   System.out.println(testProduct.price + " Expected 110");
	   System.out.println(testProduct.weight + " Expected 10");
	   
	   Regular testReg = new Regular(testStorage, 1, 2, 1);
	   
	   System.out.println(testReg.price + " Expected 0.8");
	   
	   testPurchase.addProduct(testReg);
	   
	   System.out.println(testPurchase.getTotal() + " Expected 110.8");
	   
	   testReg.switchLocation(testShelf);
	   
	   System.out.println(testReg.price + " Expected 0.72");
	   
       testPurchase.addProduct(testReg);
       
       System.out.println(testReg.price + " Expected 0.72");
	   
	   System.out.println(testPurchase.getTotal() + " Expected 111.44 due to a change in 2 items of the same substanciation");
	   
   }

}
