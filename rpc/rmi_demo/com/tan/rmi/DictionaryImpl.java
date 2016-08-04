package com.tan.rmi;

import java.io.*;
import java.rmi.RemoteException;
import java.rmi.server.UnicastRemoteObject;
import java.util.HashMap;

/**
 * Created by tan on 16-6-28.
 */
public class DictionaryImpl extends UnicastRemoteObject implements DictionaryInterface {
  private static final long serialVersionUID = -271947229644133464L;
  private String dictPath = "./com/tan/rmi/dic.txt";
  BufferedReader reader = null;
  HashMap<String, String> buffer = new HashMap<>();

  public DictionaryImpl() throws RemoteException {
    super();
    this.dictPath = "./com/tan/rmi/dic.txt";
    this.init();
  }
  public DictionaryImpl(String path) throws RemoteException {
    super();
    this.dictPath = path;
    this.init();
  }
  private void init() {
    try {
      reader = new BufferedReader(new InputStreamReader(new FileInputStream(this.dictPath)));
    } catch (Exception e) {
      e.printStackTrace();
    }
  }
  public String getOne(String key) throws RemoteException {
    System.out.println("get lookup request from client for word : " + key);
    String value = "";
    if (buffer.containsKey(key)) {
      value = buffer.get(key);
    } else {
      String line = null;
      try {
        while((line = this.reader.readLine())!=null) {
          String list[] = line.split(" ");
          if (list.length > 1) {
            if (list[0].equals(key)) {
              value = list[1];
              buffer.put(key, value);
              break;
            }
          }
        }
      } catch (IOException e) {
        e.printStackTrace();
      }
    }
    return value;
  }
}

