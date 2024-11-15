package FactoryMethod;

public class ArtDesigner extends PersonInfo implements ITStaff {
    private int level;

    public ArtDesigner() {
        super("Art Designer", "123456");
        this.level = (int)(Math.random() * 5 + 1);
        this.setStartingSalary(7000);
    }

    @Override
    public String working() { return "Art Design"; }

    @Override
    public int getSalary() { return this.getStartingSalary() + level*1500; }

    public String toString(){
        return String.format("%-12sname: %-15s, salary: %5d", working(),this.getName(),this.getSalary());
    }
}
