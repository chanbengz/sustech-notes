import javax.swing.*;
import java.awt.*;

public abstract class Shape extends JComponent {
    protected int x;
    protected int y;
    protected DrawAPI drawAPI;
    protected int gridSize;
    protected Color color;

    public Shape(int x, int y, int gridSize, DrawAPI drawAPI) {
        this.x = x;
        this.y = y;
        this.gridSize = gridSize;
        this.drawAPI = drawAPI;
        this.setLayout(null);
        this.color = new Color((int) (Math.random() * 256), (int) (Math.random() * 256), (int) (Math.random() * 256));
        this.setSize(gridSize, gridSize);
        this.setLocation(x, y);
    }

    public abstract void draw(Graphics g);

    @Override
    public void paintComponent(Graphics g) {
        super.paintComponent(g);
        g.setColor(this.color);
        draw(g);
    }

    @Override
    public int getX() {
        return x;
    }

    @Override
    public int getY() {
        return y;
    }

    public int getGridSize() {
        return gridSize;
    }

    public Color getColor() {
        return color;
    }
}
