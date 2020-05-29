
public class Regular extends Product{

   public Location local;
   public float depth;
   public float width;

   public Regular(Location location, float depth, float width, float price)
   {
	   this.depth = depth;
	   this.width = width;
	   local = location;
	   this.price = price * (1 - local.getDiscount( this ));
   }

   public void switchLocation( Location newLocation )
   {
     local = newLocation;

     price = price * (1 - local.getDiscount( this ));
   }

}
