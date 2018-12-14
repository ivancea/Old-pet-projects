package es.ivancea.beans;

import java.io.Serializable;

public class User implements Serializable{
	private static final long serialVersionUID = 1L;
	private String user;
	private String pass;
	
	public User(){
		user = pass = "";
	}

	public String getUser() {
		return user;
	}

	public void setUser(String user) {
		if(user==null)
			this.user = "";
		else
			this.user = user;
	}

	public String getPass() {
		return pass;
	}

	public void setPass(String pass) {
		if(pass==null)
			this.pass = "";
		else
			this.pass = pass;
	}
}
