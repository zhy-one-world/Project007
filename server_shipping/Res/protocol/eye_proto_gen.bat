protoc -I=.\ --cpp_out=..\..\..\faith\app\FaithGame\Source\FaithGameCore\Public\Share\Protocol\Internet .\eye_proto.proto
protogen -i:eye_proto.proto -o:..\..\..\faith\FaithEye\ShareDef\eye_proto.cs
pause
