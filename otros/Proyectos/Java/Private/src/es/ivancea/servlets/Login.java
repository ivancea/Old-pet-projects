package es.ivancea.servlets;

import java.io.IOException;
import java.io.PrintWriter;

import javax.servlet.RequestDispatcher;
import javax.servlet.ServletException;
import javax.servlet.annotation.WebServlet;
import javax.servlet.http.HttpServlet;
import javax.servlet.http.HttpServletRequest;
import javax.servlet.http.HttpServletResponse;
import javax.servlet.http.HttpSession;

import es.ivancea.beans.User;


@WebServlet(
	name = "Login",
	description = "Initial Servlet for loggining",
	urlPatterns = { "/Login" }
)
public class Login extends HttpServlet {
	private static final long serialVersionUID = 1L;
       
    public Login() {
        super();
    }

	protected void doGet(HttpServletRequest request, HttpServletResponse response) throws ServletException, IOException {
		RequestDispatcher dispatcher = request.getServletContext().getRequestDispatcher("/login.jsp");
		dispatcher.forward(request, response);
	}

	protected void doPost(HttpServletRequest request, HttpServletResponse response) throws ServletException, IOException {
		HttpSession session = request.getSession();
		User user = new User();
		user.setUser(request.getParameter("user"));
		user.setPass(request.getParameter("pass"));
		request.setAttribute("user", user);
		if(!user.getUser().equals(user.getPass())){
			RequestDispatcher dispatcher = request.getServletContext().getRequestDispatcher("/login.jsp");
			dispatcher.forward(request, response);
		}else{
			session.setAttribute("user", user.getUser());
			PrintWriter pw = response.getWriter();
			pw.println("<h1>Hello, Ivancea</h1>");
			pw.flush();
		}
	}

}
