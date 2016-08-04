package com.tan.rmi;

import java.rmi.Naming;
import java.rmi.RemoteException;
import java.rmi.registry.LocateRegistry;
/**
 * Created by tan on 16-6-28.
 */
public class Server {
  public static void main(String args[]) {
    try {
      DictionaryImpl dict = new DictionaryImpl();
      LocateRegistry.createRegistry(8080);
      Naming.bind("rmi://0.0.0.0:8080/dict", dict);
      System.out.println("server on 0.0.0.0:8080/dict");
    } catch (Exception e) {
      e.printStackTrace();
    }
  }
}
