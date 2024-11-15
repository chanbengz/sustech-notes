package FactoryMethod;

public class ArtDesignerFactory implements ITStaffFactoryInterface {
    public static ITStaff create() {
        return new ArtDesigner();
    }
}
