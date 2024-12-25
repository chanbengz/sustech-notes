import java.awt.*;

public class Rectangle extends Shape {
    private final int width;
    private final int height;
    public Rectangle(int x, int y, int gridSize, DrawAPI drawAPI, int width, int height) {
        super(x, y, gridSize, drawAPI);
        this.width = Math.min(width, gridSize);
        this.height = Math.min(height, gridSize);
    }

    @Override
    public void draw(Graphics g) {
        drawAPI.drawRectangle(g, width, height);
    }
}

