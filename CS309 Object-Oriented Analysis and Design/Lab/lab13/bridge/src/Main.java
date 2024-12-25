import javax.swing.*;
import java.util.ArrayList;
import java.util.List;

public class Main {
    public static void main(String[] args) {
        SwingUtilities.invokeLater(() -> {
            ScreenFrameTemplate screenFrameTemplate = new ScreenFrameTemplate();
            DrawAPI fillDrawAPI = new FillDrawAPI();
            DrawAPI borderDrawAPI = new BorderDrawAPI();
            List<Shape> shapes = new ArrayList<>();
//            shapes.add(new CircleFill(40, 50, 30));
//            shapes.add(new CircleBorder(240, 250, 40));
//            shapes.add(new RectangleFill(130, 10, 60, 20, 60));
//            shapes.add(new RectangleBorder(380, 30, 80,30,80));
//            shapes.add(new RectangleBorder(170, 300, 60, 60, 60));
//            shapes.add(new TriangleFill(170, 300, 60));
//            shapes.add(new TriangleBorder(270, 270, 70));

            shapes.add(new Circle(40, 50, 30, fillDrawAPI));
            shapes.add(new Circle(240, 250, 50, borderDrawAPI));
            shapes.add(new Rectangle(130, 10, 60, fillDrawAPI, 20, 60));
            shapes.add(new Rectangle(380, 30, 80, borderDrawAPI, 30, 80));
            shapes.add(new Rectangle(170, 300, 60, borderDrawAPI, 60, 60));
            shapes.add(new Triangle(170, 300, 60, fillDrawAPI));
            shapes.add(new Triangle(270, 270, 70, borderDrawAPI));
            for (Shape s : shapes) {
                screenFrameTemplate.addShapeIntoScreen(s);
            }
            screenFrameTemplate.setVisible(true);
        });
    }
}
