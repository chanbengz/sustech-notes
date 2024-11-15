package SimpleFactory;

public class Tester extends PersonInfo implements ITStaff {
	private int level;
	
	public Tester(){
		super("Tester","123456");
		super.setStartingSalary(8000);
		this.level = (int)(Math.random() * 5 + 1);
	}

	@Override
	public String working() {
		return "Testing";
	}

	@Override
	public int getSalary() {
		return super.getStartingSalary() + level * 1500;
	}
	public String toString(){
		return String.format("%-12sname: %-15s, salary: %5d", working(),this.getName(),this.getSalary());
	}
}
