@include('common.header')
@include('admin.nav')
<div class="container">
	<hr />
	<div class="row">
		<div class="col-sm-12" style="margin-bottom: 38%">
			{{ Form::open(array('route' => array('tag.store'), 'method' => 'post')) }}
			    @if ($errors)
                @foreach ($errors->all() as $message)
                <span class="text-danger">{{ $message }}</span><br />
                @endforeach
                @endif
                <div class="control-group">
                    {{ Form::label('name', '标签名称') }}
                    <div class="controls">
                    {{ Form::text('name') }}
                    </div>
                </div>
                <hr />


                <div class="form-actions">

                    {{ Form::submit('创建', array('class' => 'btn btn-info')) }}

                </div>
			{{ Form::close() }}

		</div>
	</div>

</div>

@include('admin.footer')


