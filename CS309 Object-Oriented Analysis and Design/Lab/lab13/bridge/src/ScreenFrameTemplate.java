import javax.swing.*;
import javax.swing.border.Border;
import java.awt.*;

public  class ScreenFrameTemplate extends JFrame {
    private ScreenPanel screen;

    public ScreenFrameTemplate() {
        this.setTitle("OOAD Bridge Exercise");
        this.setSize(600, 600);
        this.setLayout(null);
        this.screen = new ScreenPanel();
        this.add(screen);


        this.setLocationRelativeTo(null);
        this.setDefaultCloseOperation(EXIT_ON_CLOSE);
    }

    public void addShapeIntoScreen(Shape shape) {
        this.screen.add(shape);
        this.screen.repaint();
    }

    public void removeShapeFromScree(Shape shape){
        this.remove(shape);
        this.validate();
        this.repaint();
    }


}

class ScreenPanel extends JPanel {
    Border border = BorderFactory.createLineBorder(Color.black, 2);
    public ScreenPanel() {
        this.setSize(500, 500);
        this.setLocation(50, 30);
        this.setBackground(Color.WHITE);
        this.setBorder(border);
        this.setLayout(null);
    }

}