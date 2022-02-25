
package pack2;
import java.awt.image.BufferedImage;

public class RotateImage implements Rotation {	
    private int height;
    private int width;
    private BufferedImage image;

    RotateImage(BufferedImage image){
        this.height = image.getHeight();
        this.width = image.getWidth();
        this.image = image;
    }

    //rotire imagine 90 de grade dreapta
    public BufferedImage rotate90(){
        BufferedImage rotated = new BufferedImage(this.height, this.width, this.image.getType());
        // rotim fiecare unitate
        for(int i=0; i<this.height; i++)
            for(int j=0; j<this.width; j++)
                rotated.setRGB((this.height - 1)-i, j, image.getRGB(j, i));

        return rotated;
    }

    //rotire imagine cu 180 de grade
    public BufferedImage rotate180(){
        BufferedImage rotated = new BufferedImage(this.width, this.height, this.image.getType());

        for(int i=0; i<this.height; i++)
            for(int j=0; j<this.width; j++)
                rotated.setRGB((this.width - 1)-j, (this.height - 1)-i, image.getRGB(j, i));

        return rotated;
    }

    // rotire imagine cu 90 de grade stanga
    public BufferedImage rotate270(){
        BufferedImage rotated = new BufferedImage(this.height, this.width, this.image.getType());

        for(int i=0; i<this.height; i++)
            for(int j=0; j<this.width; j++)
                rotated.setRGB(i, (this.width - 1)-j, image.getRGB(j, i));

        return rotated;
    }
}
