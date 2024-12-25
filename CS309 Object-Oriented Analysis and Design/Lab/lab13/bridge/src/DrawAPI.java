import java.awt.*;

public interface DrawAPI {
    void drawCircle(Graphics g, int gridSize);
    void drawRectangle(Graphics g, int width, int height);
    void drawTriangle(Graphics g, int gridSize);
}