public class keyBoard {
    public static Scanner cin = new Scanner(System.in);
    
    static char a = 'a';
    static char A = 'A';

    static String str1 = "ABCDEF";
    static String str2 = "SAKLIO";

    public static void main(String[] args){

        String str = cin.nextLine();

        for(int j=0; j<str.length(); j++){

            char ch = str.charAt(j);

            for(int t=0; t<26; t++){
                if(ch == str1.charAt(t)){
                    System.out.print(str2.charAt(t));
                    break;
                }
                else if(ch == Character.toLowerCase(str1.charAt(t))){
                    System.out.print(Character.toLowerCase(str2.charAt(t)));
                    break;
                }
                else if(ch == ' '){
                    System.out.print(' ');
                    break;
                }
            }
        }
    }
}