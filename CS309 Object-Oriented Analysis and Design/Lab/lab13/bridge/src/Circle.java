import java.awt.*;

public class Circle extends Shape {
    public Circle(int x, int y, int gridSize, DrawAPI drawAPI) {super(x, y, gridSize, drawAPI);}

    public void draw(Graphics g) {
        drawAPI.drawCircle(g, gridSize);
    }
}
