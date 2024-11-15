package SimpleFactory;

public class ITStaffFactory {
    public static ITStaff createITStaff(String type) {
        return switch (type) {
            case "Developer" -> new Developer();
            case "Tester" -> new Tester();
            case "ITManager" -> new ITManager();
            default -> null;
        };
    }

    public static ITStaff createITStaff(int type) {
        return switch (type) {
            case 1 -> new ITManager();
            case 2 -> new Developer();
            case 3 -> new Tester();
            default -> null;
        };
    }
}
