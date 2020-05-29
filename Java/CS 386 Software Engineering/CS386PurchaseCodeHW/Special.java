
public class Special extends Product{

   public Location local;
   public float weight;
   public final float ADDITIONALTENPERCENT = (float) 1.10;

   public Special( Location location, float weight, float unitVal )
   {
	   this.weight = weight;
	   local = location;
	   price = ADDITIONALTENPERCENT * (weight * unitVal);
   }

   public void switchLocation( Location newLocation )
   {
     local = newLocation;
   }

}
