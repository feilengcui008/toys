package com.tan.rmi;
import java.rmi.Naming;
import java.rmi.NotBoundException;
import java.rmi.RemoteException;

public class Client {
  public static void main(String args[]) {
    DictionaryInterface dict = null;
    Client c = new Client();
    try {
      dict = (DictionaryInterface)Naming.lookup("rmi://0.0.0.0:8080/dict");
      System.out.println("get remote successfully");
    } catch(Exception e){
      e.printStackTrace();
    }
    for (int i = 1; i <= 500; ++i) {
      String key = "key" + Integer.toString(i);
      String value = c.getWord(dict, key);
      System.out.println("lookup the meaning of the word " + key + ":" + value);
    } 
  }

  private String getWord(DictionaryInterface dict, String key) {
    String value = "";
    try {
      value = dict.getOne(key);
    } catch(Exception e){
      e.printStackTrace();
    }
    return value;
  }
}
