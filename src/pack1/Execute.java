package pack1;
import java.awt.image.BufferedImage;
import java.io.File;
import java.util.Scanner;
import pack2.*;
// Radoi Constantin-Iulian
// 331 AAb 
// Proiect Rotate Image 90, 180, 270 - AWJ 2022

public class Execute extends Image{			// nivel 3 mostenire

    public static void main(String[] args) {
    	
        Scanner sc = new Scanner(System.in);	// scanner de imagine
        String newName;
        if(args.length != 0){					// daca avem args, noul nume va fi primul argument
        	newName = args[0];
        } else {
        	newName = "";						// daca nu avem args, se va citi de la tastatura noul nume la momentul cererii
        }
        
        if(newName.length() <= 0) {				
        	System.out.println("Nu a fost dat niciun nume in args.");
        	System.out.println("Se va cere nume un nume.");
        }
        
        long readingSequenceBegin = System.currentTimeMillis();
        System.out.println("Directoul curent: " + System.getProperty("user.dir"));		// afisam directorul curent pentru usurinta localizarii imaginii din assets

        System.out.println("Dati numele imaginii pe care vrem s-o rotim:");
        String insertedName = sc.next();
        
        StringBuilder stringBuilder = new StringBuilder();
        stringBuilder.append(System.getProperty("user.dir"));
        stringBuilder.append("\\" + "assets" + "\\");
        stringBuilder.append(insertedName);
        String inputString = stringBuilder.toString();
        
        System.out.println("File location will be: ");
        System.out.println(inputString);
        
        System.out.println("Dati locatia unde vrem sa salvam imaginea rotita:");
        String outputLocation = sc.next();

        String newImageName;
        if(newName.length()<= 0) 		// daca nu exista deja newName (din args, linia 13), atunci citim de la tastatura
        {
    	   System.out.println("Dati numele noii imagini:");
            newImageName = sc.next() + ".bmp";
        } else {
            newImageName = newName;
        }

        System.out.println("De cate ori rotim imaginea?");
        int rotations = sc.nextInt();
        int[] degrees = new int[rotations];		// vector cu cate unghiurile pentru fiecare rotatie

        System.out.println("Alegeti cu cate grade rotim imaginea (90, 180 sau 270):");
        for (int i = 0; i < rotations; i++){
            System.out.println("Numarul de grade pentru rotatia " + i + " - ");
        	degrees[i] = sc.nextInt();
            
        }
        sc.close();					// inchidem scannerul
        
        long readingSequenceEnd = System.currentTimeMillis();

        BufferedImage rotated;
        Image img = new Execute();		
        Buffer buffer = new Buffer();	// Construim buffer pentru Producer si consumer
        
        /////citire fisier sursa
        System.out.println("Se incepe citiera fisierului...");
        Producer producer = new Producer(buffer, inputString);		// initializam producer si constructor cu buffer si argumentele necesare
        Consumer consumer = new Consumer(buffer, outputLocation);
        long sourceReadingSequenceBegin = System.currentTimeMillis();
        
        System.out.println("O sa citim imaginea de la locatia: " + inputString);
       // img.image = img.readImage(inputString);
        producer.start();			// activam producer-ul pentru a citi imaginea si a o pune pe buffer
        img.image = buffer.get();	// luam imaginea de pe buffer
        long sourceReadingSeqeuenceEnd = System.currentTimeMillis();
        /////////////
        
        //procesare imagine
        System.out.println("Se incepe rotirea imaginii...");
        long rotationSequenceBegin = System.currentTimeMillis();
        rotated = img.rotate(degrees);	// rotim imaginea 
        long rotationSequenceEnd = System.currentTimeMillis();
        ///////////////////////////
        
        //scriere fisier destinatie
        System.out.println("Se incepe scrierea fisierului in desinatie...");
        long writingProductSequenceBegin = System.currentTimeMillis();
        img.writeImage(outputLocation, newImageName, rotated);	// scriem noua imagine rotita pe disk
        long writingProductSequenceEnd = System.currentTimeMillis();
        ////////////////////////////
        System.out.println("Proces terminat.");
        System.out.println("Etapa de citire a datelor dureaza: " + (readingSequenceEnd - readingSequenceBegin) + " milisecunde");
        System.out.println("Etapa de citire a fisierului dureaza: " + (sourceReadingSeqeuenceEnd - sourceReadingSequenceBegin) + " milisecunde");
        System.out.println("Etapa de procesare a imaginii dureaza: " + (rotationSequenceEnd - rotationSequenceBegin) + " milisecunde");
        System.out.println("Etapa de scriere a fisierului destinatie dureaza: " + (writingProductSequenceEnd  -writingProductSequenceBegin) + " milisecunde");
        System.out.println("======= DONE =======");
    }
}
