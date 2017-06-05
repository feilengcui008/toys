#!/usr/bin/env python
# -*- coding:utf-8 -*-

from command import Command

def runCmd(cmd, args):
    print cmd.name
    print args

rootCmd = Command(name="root")

serverCmd = Command("server", runCmd)
serverCmd.Parser.add_argument("-p", "--port", action='store', default="8080", help='port to listen')

clientCmd = Command("client", runCmd)
clientCmd.Parser.add_argument("-s", "--server", action="store", default=":8080",\
        help='server to connect')

subSubCmd = Command("subsub", runCmd)
subSubCmd.Parser.add_argument("-v", "--verbose", action="store_true", help="verbose mode")


rootCmd.AddCommand(serverCmd)
rootCmd.AddCommand(clientCmd)
serverCmd.AddCommand(subSubCmd)

subSubCmd.Execute()

