package FactoryMethod;

public class ITManager extends PersonInfo implements ITStaff {
	private int yearBonus;

	public ITManager(){
		super("ITManager","123456");
		this.setStartingSalary(30000);
		this.yearBonus = 3000 * (int)(Math.random() * 10);;
	}

	@Override
	public String working() {
		return "IT Manager";
	}

	@Override
	public int getSalary() {
		return super.getStartingSalary();
	}
	
	public int yearBonus(){
		return this.yearBonus;
	}

	public String toString(){
		return String.format("%-12sname: %-15s, salary: %5d, bonus in the end of year %d", 
				working(), this.getName(), this.getSalary(), this.yearBonus());
	}
}
