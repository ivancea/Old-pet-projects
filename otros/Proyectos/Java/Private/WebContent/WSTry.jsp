<%@ page language="java" contentType="text/html; charset=ISO-8859-1"
    pageEncoding="ISO-8859-1"%>
<!DOCTYPE html PUBLIC "-//W3C//DTD HTML 4.01 Transitional//EN" "http://www.w3.org/TR/html4/loose.dtd">
<html>
<head>
<meta http-equiv="Content-Type" content="text/html; charset=ISO-8859-1">
<title>Try</title>

<script>
	/// From Stackoverflow
	function gup(name) {
	  url = location.href
	  name = name.replace(/[\[]/,"\\\[").replace(/[\]]/,"\\\]");
	  var regexS = "[\\?&]"+name+"=([^&#]*)";
	  var regex = new RegExp( regexS );
	  var results = regex.exec( url );
	  return results == null ? null : results[1];
	}
	
	var t = gup("text")
	var ws = new WebSocket("ws://87.223.69.178:8080/Private/chat/"+(t==null?"Pon: \"text%3DX\"":t));
	ws.onmessage = function(evt){
		alert(evt.data);
	}
</script>

</head>
<body>

</body>
</html>