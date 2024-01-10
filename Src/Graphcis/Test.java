import java.awt.*;
import javax.swing.*;
import javax.xml.stream.Location;

public class Test {

    public void createPlayground() {
        JFrame frame = new JFrame("ForFun Maze");
        frame.setResizable(false);
        frame.setLayout(new BorderLayout());

        JPanel buttonPanel = new JPanel();
        buttonPanel.setLayout(new GridLayout(3, 1));
        buttonPanel.setPreferredSize(new Dimension(772, 441));
        buttonPanel.setMaximumSize(new Dimension(772, 441));

        JButton tcpOP = new JButton();
        tcpOP.setText("TCP Optiminzation");
        tcpOP.setSize(140, 252);
        
        JButton storageOP = new JButton();
        storageOP.setText("Storage Opminization");
        storageOP.setSize(140, 152);

        buttonPanel.add(tcpOP);
        buttonPanel.add(storageOP);

        Location[][] array = null;
        JPanel centerPanel = new JPanel();
        centerPanel.setPreferredSize(new Dimension(600, 600));
        centerPanel.setMinimumSize(new Dimension(600, 600));

        JPanel leftPanel = new JPanel();
        leftPanel.setPreferredSize(new Dimension(100, 600));
        JPanel northPanel = new JPanel();
        northPanel.setPreferredSize(new Dimension(800, 100));
        JPanel bottomPanel = new JPanel();
        bottomPanel.setPreferredSize(new Dimension(800, 100));

        frame.add(northPanel, BorderLayout.NORTH);
        frame.add(leftPanel, BorderLayout.WEST);
        frame.add(centerPanel, BorderLayout.CENTER);
        frame.add(buttonPanel, BorderLayout.EAST);
        frame.add(bottomPanel, BorderLayout.SOUTH);

        frame.setDefaultCloseOperation(JFrame.EXIT_ON_CLOSE);
        frame.pack();
        frame.setLocationRelativeTo(null);
        frame.setVisible(true);
        System.out.println("Size of centerpane" + centerPanel.getWidth() + "x" + centerPanel.getHeight());
    }

    public static void main(String[] args) {
        SwingUtilities.invokeLater(new Runnable() {

            @Override
            public void run() {
                new Test().createPlayground();
            }
        });
    }
}