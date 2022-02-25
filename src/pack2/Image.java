package pack2;

import java.io.File;
import java.io.IOException;
import javax.imageio.ImageIO;
import java.awt.image.BufferedImage;

public class Image extends ReadWriteImage {		// nivel 2
    public BufferedImage image;

    @Override
    public BufferedImage readImage(String path) {
        File input = new File(path);
        try{
            image = ImageIO.read(input);
        }
        catch(IOException ex){
        	System.out.println("Eroare la citirea imaginii.");
            ex.printStackTrace();
        }
        return image;
    }

    @Override
    public void writeImage(String path, String string_name, BufferedImage rotated) {
        File output = new File(path, string_name);
        try{
            ImageIO.write(rotated, "bmp", output);
            System.out.println("Finished!");
        }
        catch (IOException ex){
        	System.out.println("Eroare la scrierea imaginii.");
            ex.printStackTrace();
        }
    }

    @Override
    public BufferedImage rotate(int ...degrees){	// varargs
        RotateImage rot = new RotateImage(this.image);
        BufferedImage rotated = this.image;
        int sum = 0;

        for (int degree : degrees) sum += degree;	// adunam toate gradele date de utilizator pentru si verificam daca sunt 90,180,0,270

        switch (sum%360){
            case 90:
                rotated = rot.rotate90();
                break;
            case 180:
                rotated = rot.rotate180();
                break;
            case 270:
                rotated = rot.rotate270();
                break;
            case 0:
                rotated = this.image;
                break;
            default:
                System.out.println("Unghi introdus gresit. Va rugam sa introduceti 90, 180 sau 270!");
        }

        return rotated;
    }
}