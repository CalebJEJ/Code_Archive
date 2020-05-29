public class Shelf extends Location{

   public Shelf()
   {
	   discount = (float) 0.1;
   }

   public float getDiscount( Product  self )
   {
      return getRegDiscount((Regular) self);
   }
   
   public float getRegDiscount (Regular self) 
   {
	   return (self.width * self.depth) % discount;
   }
   
}
