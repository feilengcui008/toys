@include('common.header')
@include('admin.nav')
<div class="container">
	<hr />
	<div class="row">
		<div class="col-sm-12" style="margin-bottom: 23%">
			{{ Form::open(array('route' => array('tag.update', $tag->id), 'method' => 'put')) }}
			    @if ($errors)
                @foreach ($errors->all() as $message)
                <span class="text-danger">{{ $message }}</span><br />
                @endforeach
                @endif
                <div class="control-group">
                    {{ Form::label('ID', '标签ID') }}:{{ Form::label('ID', $tag->id) }}
                </div>
                <hr />
                <div class="control-group">
                    {{ Form::label('name', '标签名称') }}
                    <div class="controls">
                    {{ Form::text('name',$tag->name) }}
                    </div>
                </div>
                <hr />

                <div class="control-group">
                    {{ Form::label('article_ids', '文章ID') }}(如：1,2,3 逗号分割的字符串的形式)
                    <div class="controls">
                    {{ Form::text('article_ids',$tag->article_ids) }}
                    </div>
                </div>
                <hr />

                <div class="form-actions">

                    {{ Form::submit('修改', array('class' => 'btn btn-info')) }}

                </div>
			{{ Form::close() }}

		</div>
	</div>

</div>

@include('admin.footer')


