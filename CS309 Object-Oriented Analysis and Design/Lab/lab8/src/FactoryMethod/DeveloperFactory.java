package FactoryMethod;

public class DeveloperFactory implements ITStaffFactoryInterface {
    public static ITStaff create() {
        return new Developer();
    }
}
