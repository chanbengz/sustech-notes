import java.awt.*;

public class Triangle extends Shape {
    public Triangle(int x, int y, int gridSize, DrawAPI drawAPI) {
        super(x, y, gridSize, drawAPI);
    }

    public void draw(Graphics g) {
        drawAPI.drawTriangle(g, gridSize);
    }
}
