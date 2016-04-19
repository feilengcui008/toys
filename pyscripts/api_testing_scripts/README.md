a simple script to test backend rest API for json, a little change can made to support put, delete etc..., usually testing for a Web backend API can be a lot of work, actually we can store the testcases in json file to automate and reduce the work.

parameter required by the josn testcase files 
```
{
  "url" : "http://test.com/"
  "method" : "get"
  "data" : {
    "key1" : "value1",
    "key2" : "value2"
  }
}
```

usage: python test.py test1.json
