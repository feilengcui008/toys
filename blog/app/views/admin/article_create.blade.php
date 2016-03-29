@include('common.header')
@include('admin.nav')
<div class="container">
	<hr />
	<div class="row">
		<div class="col-sm-12">
			{{ Form::open(array('route' => array('article.store'), 'method' => 'post')) }}
			    @if ($errors)
                @foreach ($errors->all() as $message)
                <span class="text-danger">{{ $message }}</span><br />
                @endforeach
                @endif
                <div class="control-group">
                    {{ Form::label('title', '文章标题') }}
                    <div class="controls">
                    {{ Form::text('title') }}
                    </div>
                </div>
                <hr />
                <div class="control-group">
                                    {{ Form::label('title', '文章描述') }}
                                    <div class="controls">
                                    {{ Form::text('description') }}
                                    </div>
                                </div>
                <hr />
                <div class="control-group">
                    {{ Form::label('tags', '文章标签') }}(如：1,2,3 逗号分割的字符串的形式)
                    <div class="controls">
                    {{ Form::text('tags') }}
                    </div>
                </div>
                <hr />
                <div class="control-group">
                    {{ Form::label('content', '文章内容') }}
                    <div class="controls">
                    {{ Form::textarea('content','',array('id'=>'textdomain')) }}
                    </div>
                </div>
                <hr />
                <div class="form-actions">

                    {{ Form::submit('添加',array('class' => 'btn btn-info')) }}

                </div>
			{{ Form::close() }}

		</div>
	</div>

</div>

@include('admin.footer')


