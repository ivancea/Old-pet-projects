package es.ivancea.beans;

import javax.servlet.http.HttpSession;
import javax.websocket.HandshakeResponse;
import javax.websocket.server.HandshakeRequest;
import javax.websocket.server.ServerEndpointConfig;

public class HttpSessionGetter extends ServerEndpointConfig.Configurator{
	@Override
	public void modifyHandshake(ServerEndpointConfig config, 
					            HandshakeRequest request, 
					            HandshakeResponse response){
		config.getUserProperties()
			  .put(HttpSession.class.getName(), request.getHttpSession());
	}
}
