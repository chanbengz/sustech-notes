package SimpleFactory;

public class PersonInfo {
	private  String userID;
	private String name;
	private String password;
	private int StartingSalary;
	
	private static int number = 1;

	public PersonInfo() {
		userID = String.valueOf(10000+number);
		number++;
	}

	public PersonInfo (String name, String password) {
		this.userID = String.valueOf(10000 + number);
		this.name = userID + " " + name;
		this.password = password;
		number++;
	}

	public void setName(String name) {
		this.name = name;
	}
	public void setPassword(String password) {
		this.password = password;
	}
	public String getUserID() {
		return userID;
	}
	public String getName() {
		return name;
	}
	public String getPassword() {
		return password;
	}
	public int getStartingSalary() {
		return StartingSalary;
	}
	public void setStartingSalary(int i) {
		StartingSalary = i;
	}
}
