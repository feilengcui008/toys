package com.tan.rmi;

import java.rmi.Remote;
import java.rmi.RemoteException;
/**
 * Created by tan on 16-6-28.
 */
public interface DictionaryInterface extends Remote {
  public String getOne(String key) throws RemoteException;
}
