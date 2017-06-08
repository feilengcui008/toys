#!/usr/bin/env python
# -*- coding:utf-8 -*-

import argparse


class CommandException(Exception):
    pass


class Command(object):
    name = ""
    commands = []
    run = None
    parser = None
    subparsers = None
    parent = None

    def __init__(self, name=None, run=None):
        self.name = name
        self.run = run
        self.parser = argparse.ArgumentParser()
        self.parser._cmd = self

    def Execute(self):
        # goto root cmd
        if self.parent:
            return self.parent.Execute()
        # find the command
        args = self.parser.parse_args()
        parserName = args.__dict__["_lastMatchedParser"]
        cmd = self
        if parserName != cmd.name:
            cmd = self._getParser(parserName)
        if cmd.run:
            cmd.run(cmd, args)

    def _getParser(self, name):
        for k, v in self.subparsers._name_parser_map.iteritems():
            if k == name:
                return v._cmd
            t = v._cmd._getParser(name)
            if t is not None:
                return t
        return None

    def AddCommand(self, cmd, name=None):
        if not isinstance(cmd, Command):
            raise CommandException("need Command type when AddCommand")
        for command in self.commands:
            if command.name == cmd.name:
                raise CommandException(
                    "subcommand with the same name already exists")
        if not self.subparsers:
            self.subparsers = self.parser.add_subparsers(
                dest="_lastMatchedParser")
        cmdName = name if name is not None else cmd.name
        if not cmdName or not isinstance(cmdName, str) or cmdName == "":
            raise CommandException("subcommand name not valid")
        if cmdName in self.subparsers._name_parser_map.keys():
            raise CommandException(
                "subcommand of name %s already exists" % (cmdName,))
        self.subparsers._name_parser_map[cmdName] = cmd.parser
        self.commands.append(cmd)
        cmd.parent = self

    @property
    def Parser(self):
        return self.parser
