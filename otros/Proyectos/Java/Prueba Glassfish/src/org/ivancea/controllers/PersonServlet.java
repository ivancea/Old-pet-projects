package org.ivancea.controllers;

import java.io.IOException;

import javax.servlet.RequestDispatcher;
import javax.servlet.ServletException;
import javax.servlet.annotation.WebServlet;
import javax.servlet.http.HttpServlet;
import javax.servlet.http.HttpServletRequest;
import javax.servlet.http.HttpServletResponse;

import org.ivancea.models.Person;


@WebServlet("/PersonServlet")
public class PersonServlet extends HttpServlet {
	private static final long serialVersionUID = 1L;
	
	private String name;
	private int age;
	private String personID = "personBean";
       
    public PersonServlet() {
        super();
    }

	protected void doGet(HttpServletRequest request, HttpServletResponse response) throws ServletException, IOException {
		name = request.getParameter("name");
		try{
			age = Integer.valueOf(request.getParameter("age"));
		}catch(NumberFormatException e){
			age = 0;
		}
		
		Person person = new Person();
		person.setName(name);
		person.setAge(age);
		
		//HttpSession session = request.getSession();
		
		request.setAttribute(personID, person);
		RequestDispatcher dispatcher = getServletContext().getRequestDispatcher("/userAgeResponse.jsp");
		dispatcher.forward(request, response);
	}

}
