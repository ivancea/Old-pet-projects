package es.ivancea.endpoints;

import java.io.IOException;

import javax.servlet.http.HttpSession;
import javax.websocket.EndpointConfig;
import javax.websocket.OnOpen;
import javax.websocket.Session;
import javax.websocket.server.PathParam;
import javax.websocket.server.ServerEndpoint;

import es.ivancea.beans.HttpSessionGetter;

@ServerEndpoint(value = "/chat/{msg}", configurator = HttpSessionGetter.class)
public class PruebaWS {
	
	public HttpSession httpSession;
	
	@OnOpen
	public void open(final Session session,
					 EndpointConfig config,
			         @PathParam("msg") final String msg){
		httpSession = (HttpSession)config.getUserProperties().get(HttpSession.class.getName());
		try {
			session.getBasicRemote().sendText(msg);
		} catch (IOException e) {
			e.printStackTrace();
		}
	}
}
