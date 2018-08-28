import java.util.*;

public class HelloWorld {

    public static void main(String args[]) {

        Scanner sb = new Scanner(System.in);

        System.out.print("输入你的数字(nextLine!)：");
        String num1 = sb.nextLine();

        Integer convert_num1 = 0;

        try{
            convert_num1 = Integer.parseInt(num1);
        }catch(NumberFormatException e){}

        System.out.print("你输入的数是：" + convert_num1 + "\n");

        System.out.print("输入你的数字(nextInt!)：");
        int num2 = sb.nextInt();
        try{
            convert_num1 = Integer.parseInt(num1);
        }catch(NumberFormatException e){
            num2 = 0;
        }
        
        System.out.print("你输入的数是：" + num2 + "\n");

        sb.close();
    }

}