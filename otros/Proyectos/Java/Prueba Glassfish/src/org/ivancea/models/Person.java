package org.ivancea.models;

import java.io.Serializable;


public class Person implements Serializable{
	private static final long serialVersionUID = 1L;
	private String name;
	private int age;
	
	public Person(){
		setName("");
		setAge(0);
	}

	public String getName() {
		return name;
	}

	public void setName(String name) {
		this.name = name;
	}

	public int getAge() {
		return age;
	}

	public void setAge(int age) {
		this.age = age;
	}
}