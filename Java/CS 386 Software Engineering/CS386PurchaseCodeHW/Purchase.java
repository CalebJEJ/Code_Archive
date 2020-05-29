
public class Purchase{

   public Product[] list;
   public int numProducts;


   public Purchase(int size)
   {
	   list = new Product[size];
	   numProducts = 0;
   }

   public void addProduct(Product p)
   {
	   list[numProducts] = p;
	   numProducts++;
   }
   
   public float getTotal()
   {
	   float price = 0;
	   
	   for(int i=0; i < numProducts; i++)
	   {
		   price += list[i].price;
	   }
	   return price;
   }

}
