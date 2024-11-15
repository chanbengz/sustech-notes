package FactoryMethod;

public class Developer extends PersonInfo implements ITStaff {
	private int level;
	
	public Developer(){
		super("Developer","123456");
		this.setStartingSalary(10000);
		this.level = (int)(Math.random() * 5 + 1);
	}
	@Override
	public String working() {
		return "Coding";
	}

	@Override
	public int getSalary() {
		return super.getStartingSalary() + level * 2000;
	}

	public String toString(){
		return String.format("%-12sname: %-15s, salary: %5d", working(), this.getName(), this.getSalary());
	}
}
