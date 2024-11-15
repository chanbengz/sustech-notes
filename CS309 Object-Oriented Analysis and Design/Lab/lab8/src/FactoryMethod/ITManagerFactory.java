package FactoryMethod;

public class ITManagerFactory implements ITStaffFactoryInterface {
    public static ITStaff create() {
        return new ITManager();
    }
}
