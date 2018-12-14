<%@ page language="java" contentType="text/html; charset=ISO-8859-1"
    pageEncoding="ISO-8859-1"%>
<!DOCTYPE html PUBLIC "-//W3C//DTD HTML 4.01 Transitional//EN" "http://www.w3.org/TR/html4/loose.dtd">
<jsp:useBean id="user" class="es.ivancea.beans.User" scope="request" />
<html>
<head>
<meta http-equiv="Content-Type" content="text/html; charset=ISO-8859-1">
<title>Login</title>
</head>
<body>
	<form method="POST">
		User: <input type="text" name="user" value="${user.user}" /><br/>
		Password: <input type="text" name="pass" value="${user.pass}" /><br/>
		<input type="submit" value="Login" />
	</form>
</body>
</html>