public class GeometryTest
{
    public static void main(String[] args)
    {
        Point p1 = new Point(3, 4);
        Point p2 = new Point(5, -2);
        Rectangle r1 = new Rectangle(p1, p2);
        Point p3 = r1.get_top_left();
        Point p4 = r1.get_top_right();
        Point p5 = r1.get_bottom_left();
        Point p6 = r1.get_bottom_right();

        System.out.println("r1 = ["
                           +"("+p3.get_x()+","+p3.get_y()+"),"
                           +"("+p4.get_x()+","+p4.get_y()+"),"
                           +"("+p5.get_x()+","+p5.get_y()+"),"
                           +"("+p6.get_x()+","+p6.get_y()+")]");

        System.out.print("r1 = ");   
        r1.print();
        System.out.print("p2 = ");
        p2.print();
        System.out.print("  p6 = ");
        p6.print();
        System.out.println();
        System.out.println("test1: "+p2.equals(p6) + ", " +p2.same_as(p6));

        p2.translate(2,-1);
        System.out.print("r1 = ");   
        r1.print();
        System.out.print("p2 = ");
        p2.print();
        System.out.print("  p6 = ");
        p6.print();
        System.out.println();
        System.out.println("test2: "+p2.equals(p6) + ", " +p2.same_as(p6));
        
        Point p7 = new Point(11, -5);
        Rectangle r2 = new Rectangle(p6, p7);
        System.out.print("r2 = ");   
        r2.print();

        Point p8 = r2.get_top_left();
        System.out.print("p2 = ");
        p2.print();
        System.out.print("  p6 = ");
        p6.print();
        System.out.print("  p8 = ");
        p8.print();
        System.out.println();
        System.out.println("test3: "+p2.equals(p6) + ", " +p2.same_as(p6));
        System.out.println("test4: "+p2.equals(p8) + ", " +p2.same_as(p8));
        
        p8.translate(-3,2);
        System.out.print("r1 = ");   
        r1.print();
        System.out.print("r2 = ");   
        r2.print();
        
        Point p9 = new Point(4,-1);
        System.out.print("p2 = ");
        p2.print();
        System.out.print("  p6 = ");
        p6.print();
        System.out.print("  p8 = ");
        p8.print();
        System.out.print("  p9 = ");
        p9.print();
        System.out.println();
        System.out.println("test5: "+p2.equals(p6) + ", " +p2.same_as(p9));
        
        Point p10 = new Point(4,-1);
        Point p11 = r2.get_bottom_right();
        Rectangle r3 = new Rectangle(p10, p11);
        System.out.print("r2 = ");   
        r2.print();
        System.out.print("r3 = ");   
        r3.print();
        System.out.println("test6: "+r2.equals(r3) + ", "+r2.same_as(r3));

        Point p12 = new Point(5,-2), p13 = new Point(9,-3);
        Rectangle r4 = new Rectangle(p12, p13);
        System.out.print("r4 = ");
        r4.print();
        System.out.println("test7: "+r4.contained_in(r3) + ", "
                           + r4.contains(r3) + ", "
                           + r3.contained_in(r4) + ", "
                           + r3.contains(r4));

        r2.translate(new Point(0,0));
        System.out.print("r2 = ");   
        r2.print();
        System.out.print("r3 = ");   
        r3.print();
        System.out.println("test8: "+r2.equals(r3) + ", "+r2.same_as(r3));
        
        System.out.print("p2 = ");
        p2.print();
        System.out.print("  p6 = ");
        p6.print();
        System.out.print("  p8 = ");
        p8.print();
        System.out.print("  p9 = ");
        p9.print();
        System.out.println();
        System.out.println("test9: "+p2.equals(p6) + ", " +p2.same_as(p9));
    }
}
