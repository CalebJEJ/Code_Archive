
public class Storage extends Location{

   public Storage()
   {
	   discount = (float) 0.2;
   }

   public float getDiscount( Product self )
   {
     return discount;
   }

}
