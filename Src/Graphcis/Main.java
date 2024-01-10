//https://stackoverflow.com/questions/46483075/swing-vertical-navigation-menu
import java.awt.*;
import java.awt.event.*;
import javax.swing.*;

public class Main extends JFrame {
   public Main(){
      setLayout(new BorderLayout());
   
      setBackground(Color.red);
   
      Button tcpOP = new Button("TCP Optimization");
      Button storageOP = new Button("Storage Optimization");
      Button as = new Button("");
      add(tcpOP);
      add(storageOP);
      add(as);
      tcpOP.setBounds(30,30,150,50); 
      storageOP.setBounds(30,80,150,50); 
   
      setTitle("Test");
      setSize(972, 641);
      setVisible(true);
   }
 
   // Main Method
   public static void main(String args[]){
      new Main();
   }
}