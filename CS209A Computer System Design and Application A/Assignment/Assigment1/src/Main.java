import java.nio.file.Paths;

public class Main {
    public static void main(String[] args) {
        String currentDirectory = System.getProperty("user.dir");
        System.out.println("当前工作目录是: " + currentDirectory);
        OlympicsAnalyzer olympicsAnalyzer = new OlympicsAnalyzer(Paths.get("src", "local_test_data").toString());
    }
}
