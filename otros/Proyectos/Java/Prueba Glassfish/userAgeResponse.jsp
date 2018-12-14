<%@ page language="java" contentType="text/html; charset=ISO-8859-1"
    pageEncoding="ISO-8859-1"%>
<!DOCTYPE html PUBLIC "-//W3C//DTD HTML 4.01 Transitional//EN" "http://www.w3.org/TR/html4/loose.dtd">

<jsp:useBean id="personBean" scope="request" class="org.ivancea.models.Person" />

<html>
<head>
<meta http-equiv="Content-Type" content="text/html; charset=ISO-8859-1">
<title>${personBean.name} - ${personBean.age}</title>
</head>
<body>
	Hello, ${personBean.name}.
	<br>
	You have ${personBean.age} years.
</body>
</html>