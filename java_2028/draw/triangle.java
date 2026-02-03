package fr.epita.assistants.drawing;

public class Triangle extends Sharp {

    public Triangle(int length) {
        super(length);
    }
    
    @Override
    public void draw() {
        for (int ligne = 0; ligne < length; ligne++) {
            for (int colonne = 0; colonne <= ligne; colonne++) {
                if (colonne == 0 || colonne == ligne || ligne == length - 1) {
                    System.out.print("#");
                } else {
                    System.out.print(" ");  
                }
                
                if (colonne < ligne) {
                    System.out.print(" ");
                }
            }
            
            if (ligne < length - 1) {
                System.out.println();
            }
        }
    }
}
