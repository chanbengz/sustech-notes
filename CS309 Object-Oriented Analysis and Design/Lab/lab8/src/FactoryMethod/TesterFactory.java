package FactoryMethod;

public class TesterFactory implements ITStaffFactoryInterface {
    public static ITStaff create() {
        return new Tester();
    }
}
