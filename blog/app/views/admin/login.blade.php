@extends('common.default')


@section('content')
 
  <div class="col-sm-4 col-sm-offset-4 login">
 
    {{ Form::open() }}
 
      @if ($errors->has('login'))
 
        <div class="alert alert-error">{{ $errors->first('login', ':message') }}</div>
       
      @endif
 
      <div class="control-group">
         
        {{ Form::label('email', '邮箱') }}
         
        <div class="controls">
           
          {{ Form::text('email') }}
         
        </div>
      </div>
 
      <div class="control-group">
         
        {{ Form::label('password', '密码') }}
         
        <div class="controls">
           
          {{ Form::password('password') }}
         
        </div>
      </div>
      <hr />
      <div class="form-actions" align="center">
         
        {{ Form::submit('Login', array('class' => 'btn btn-primary')) }}
       
      </div>
 
    {{ Form::close() }}
   
  </div>
 


@stop