; ModuleID = 'demo.c'
source_filename = "demo.c"
target datalayout = "e-m:e-i64:64-f80:128-n8:16:32:64-S128"
target triple = "x86_64-pc-linux-gnu"

@gvar = global i32 20, align 4, !dbg !0
@.str = private unnamed_addr constant [12 x i8] c"%d %d %d %d\00", align 1
@.str.1 = private unnamed_addr constant [4 x i8] c"%d\0A\00", align 1

; Function Attrs: noinline nounwind optnone uwtable
define i32 @main() #0 !dbg !13 {
  %1 = alloca i32, align 4
  %2 = alloca i32, align 4
  %3 = alloca i32*, align 8
  %4 = alloca i32, align 4
  %5 = alloca i32, align 4
  %6 = alloca i32, align 4
  %7 = alloca i32, align 4
  store i32 0, i32* %1, align 4
  call void @llvm.dbg.declare(metadata i32* %2, metadata !16, metadata !DIExpression()), !dbg !17
  store i32 55295998, i32* %2, align 4, !dbg !17
  call void @llvm.dbg.declare(metadata i32** %3, metadata !18, metadata !DIExpression()), !dbg !19
  %8 = call noalias i8* @malloc(i64 40) #5, !dbg !20
  %9 = bitcast i8* %8 to i32*, !dbg !21
  store i32* %9, i32** %3, align 8, !dbg !19
  %10 = load i32*, i32** %3, align 8, !dbg !22
  %11 = getelementptr inbounds i32, i32* %10, i64 4, !dbg !22
  store i32 10, i32* %11, align 4, !dbg !23
  %12 = bitcast i32* %2 to i8*, !dbg !24
  call void @llvm.prefetch(i8* %12, i32 0, i32 0, i32 1), !dbg !25
  call void @llvm.prefetch(i8* bitcast (i32* @gvar to i8*), i32 0, i32 0, i32 1), !dbg !26
  call void @llvm.dbg.declare(metadata i32* %4, metadata !27, metadata !DIExpression()), !dbg !28
  call void @llvm.dbg.declare(metadata i32* %5, metadata !29, metadata !DIExpression()), !dbg !30
  call void @llvm.dbg.declare(metadata i32* %6, metadata !31, metadata !DIExpression()), !dbg !32
  call void @llvm.dbg.declare(metadata i32* %7, metadata !33, metadata !DIExpression()), !dbg !34
  %13 = call i32 (i8*, ...) @__isoc99_scanf(i8* getelementptr inbounds ([12 x i8], [12 x i8]* @.str, i32 0, i32 0), i32* %4, i32* %5, i32* %6, i32* %7), !dbg !35
  %14 = load i32, i32* %4, align 4, !dbg !36
  %15 = icmp sgt i32 %14, 0, !dbg !38
  br i1 %15, label %16, label %22, !dbg !39

; <label>:16:                                     ; preds = %0
  %17 = load i32, i32* %5, align 4, !dbg !40
  %18 = icmp slt i32 %17, 10, !dbg !41
  br i1 %18, label %19, label %22, !dbg !42

; <label>:19:                                     ; preds = %16
  %20 = load i32, i32* %6, align 4, !dbg !43
  %21 = icmp sgt i32 %20, 10, !dbg !44
  br label %22

; <label>:22:                                     ; preds = %19, %16, %0
  %23 = phi i1 [ false, %16 ], [ false, %0 ], [ %21, %19 ]
  %24 = zext i1 %23 to i32, !dbg !42
  %25 = load i32, i32* %7, align 4, !dbg !45
  %26 = icmp slt i32 %25, 0, !dbg !46
  br i1 %26, label %27, label %31, !dbg !47

; <label>:27:                                     ; preds = %22
  %28 = load i32, i32* %5, align 4, !dbg !48
  %29 = load i32, i32* %2, align 4, !dbg !50
  %30 = add nsw i32 %29, %28, !dbg !50
  store i32 %30, i32* %2, align 4, !dbg !50
  br label %31, !dbg !51

; <label>:31:                                     ; preds = %27, %22
  %32 = load i32, i32* %2, align 4, !dbg !52
  %33 = load i32, i32* %5, align 4, !dbg !53
  %34 = mul nsw i32 %32, %33, !dbg !54
  %35 = load i32, i32* @gvar, align 4, !dbg !55
  %36 = load i32, i32* %5, align 4, !dbg !56
  %37 = sdiv i32 %35, %36, !dbg !57
  %38 = sub nsw i32 %34, %37, !dbg !58
  %39 = load i32*, i32** %3, align 8, !dbg !59
  %40 = getelementptr inbounds i32, i32* %39, i64 4, !dbg !59
  %41 = load i32, i32* %40, align 4, !dbg !59
  %42 = add nsw i32 %38, %41, !dbg !60
  %43 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([4 x i8], [4 x i8]* @.str.1, i32 0, i32 0), i32 %42), !dbg !61
  ret i32 0, !dbg !62
}

; Function Attrs: nounwind readnone speculatable
declare void @llvm.dbg.declare(metadata, metadata, metadata) #1

; Function Attrs: nounwind
declare noalias i8* @malloc(i64) #2

; Function Attrs: inaccessiblemem_or_argmemonly nounwind
declare void @llvm.prefetch(i8* nocapture readonly, i32, i32, i32) #3

declare i32 @__isoc99_scanf(i8*, ...) #4

declare i32 @printf(i8*, ...) #4

attributes #0 = { noinline nounwind optnone uwtable "correctly-rounded-divide-sqrt-fp-math"="false" "disable-tail-calls"="false" "less-precise-fpmad"="false" "no-frame-pointer-elim"="true" "no-frame-pointer-elim-non-leaf" "no-infs-fp-math"="false" "no-jump-tables"="false" "no-nans-fp-math"="false" "no-signed-zeros-fp-math"="false" "no-trapping-math"="false" "stack-protector-buffer-size"="8" "target-cpu"="x86-64" "target-features"="+fxsr,+mmx,+sse,+sse2,+x87" "unsafe-fp-math"="false" "use-soft-float"="false" }
attributes #1 = { nounwind readnone speculatable }
attributes #2 = { nounwind "correctly-rounded-divide-sqrt-fp-math"="false" "disable-tail-calls"="false" "less-precise-fpmad"="false" "no-frame-pointer-elim"="true" "no-frame-pointer-elim-non-leaf" "no-infs-fp-math"="false" "no-nans-fp-math"="false" "no-signed-zeros-fp-math"="false" "no-trapping-math"="false" "stack-protector-buffer-size"="8" "target-cpu"="x86-64" "target-features"="+fxsr,+mmx,+sse,+sse2,+x87" "unsafe-fp-math"="false" "use-soft-float"="false" }
attributes #3 = { inaccessiblemem_or_argmemonly nounwind }
attributes #4 = { "correctly-rounded-divide-sqrt-fp-math"="false" "disable-tail-calls"="false" "less-precise-fpmad"="false" "no-frame-pointer-elim"="true" "no-frame-pointer-elim-non-leaf" "no-infs-fp-math"="false" "no-nans-fp-math"="false" "no-signed-zeros-fp-math"="false" "no-trapping-math"="false" "stack-protector-buffer-size"="8" "target-cpu"="x86-64" "target-features"="+fxsr,+mmx,+sse,+sse2,+x87" "unsafe-fp-math"="false" "use-soft-float"="false" }
attributes #5 = { nounwind }

!llvm.dbg.cu = !{!2}
!llvm.module.flags = !{!9, !10, !11}
!llvm.ident = !{!12}

!0 = !DIGlobalVariableExpression(var: !1, expr: !DIExpression())
!1 = distinct !DIGlobalVariable(name: "gvar", scope: !2, file: !3, line: 18, type: !7, isLocal: false, isDefinition: true)
!2 = distinct !DICompileUnit(language: DW_LANG_C99, file: !3, producer: "clang version 6.0.0-1ubuntu2 (tags/RELEASE_600/final)", isOptimized: false, runtimeVersion: 0, emissionKind: FullDebug, enums: !4, retainedTypes: !5, globals: !8)
!3 = !DIFile(filename: "demo.c", directory: "/home/wzl/SourceCodeAnalysis/cvarpass/testset")
!4 = !{}
!5 = !{!6}
!6 = !DIDerivedType(tag: DW_TAG_pointer_type, baseType: !7, size: 64)
!7 = !DIBasicType(name: "int", size: 32, encoding: DW_ATE_signed)
!8 = !{!0}
!9 = !{i32 2, !"Dwarf Version", i32 4}
!10 = !{i32 2, !"Debug Info Version", i32 3}
!11 = !{i32 1, !"wchar_size", i32 4}
!12 = !{!"clang version 6.0.0-1ubuntu2 (tags/RELEASE_600/final)"}
!13 = distinct !DISubprogram(name: "main", scope: !3, file: !3, line: 19, type: !14, isLocal: false, isDefinition: true, scopeLine: 19, isOptimized: false, unit: !2, variables: !4)
!14 = !DISubroutineType(types: !15)
!15 = !{!7}
!16 = !DILocalVariable(name: "lvar", scope: !13, file: !3, line: 20, type: !7)
!17 = !DILocation(line: 20, column: 6, scope: !13)
!18 = !DILocalVariable(name: "hvar", scope: !13, file: !3, line: 22, type: !6)
!19 = !DILocation(line: 22, column: 7, scope: !13)
!20 = !DILocation(line: 22, column: 21, scope: !13)
!21 = !DILocation(line: 22, column: 14, scope: !13)
!22 = !DILocation(line: 23, column: 2, scope: !13)
!23 = !DILocation(line: 23, column: 10, scope: !13)
!24 = !DILocation(line: 24, column: 21, scope: !13)
!25 = !DILocation(line: 24, column: 2, scope: !13)
!26 = !DILocation(line: 25, column: 2, scope: !13)
!27 = !DILocalVariable(name: "a", scope: !13, file: !3, line: 28, type: !7)
!28 = !DILocation(line: 28, column: 6, scope: !13)
!29 = !DILocalVariable(name: "b", scope: !13, file: !3, line: 28, type: !7)
!30 = !DILocation(line: 28, column: 8, scope: !13)
!31 = !DILocalVariable(name: "c", scope: !13, file: !3, line: 28, type: !7)
!32 = !DILocation(line: 28, column: 10, scope: !13)
!33 = !DILocalVariable(name: "d", scope: !13, file: !3, line: 28, type: !7)
!34 = !DILocation(line: 28, column: 12, scope: !13)
!35 = !DILocation(line: 29, column: 2, scope: !13)
!36 = !DILocation(line: 30, column: 9, scope: !37)
!37 = distinct !DILexicalBlock(scope: !13, file: !3, line: 30, column: 9)
!38 = !DILocation(line: 30, column: 11, scope: !37)
!39 = !DILocation(line: 30, column: 14, scope: !37)
!40 = !DILocation(line: 30, column: 17, scope: !37)
!41 = !DILocation(line: 30, column: 19, scope: !37)
!42 = !DILocation(line: 30, column: 24, scope: !37)
!43 = !DILocation(line: 30, column: 27, scope: !37)
!44 = !DILocation(line: 30, column: 29, scope: !37)
!45 = !DILocation(line: 30, column: 34, scope: !37)
!46 = !DILocation(line: 30, column: 35, scope: !37)
!47 = !DILocation(line: 30, column: 9, scope: !13)
!48 = !DILocation(line: 31, column: 17, scope: !49)
!49 = distinct !DILexicalBlock(scope: !37, file: !3, line: 30, column: 38)
!50 = !DILocation(line: 31, column: 14, scope: !49)
!51 = !DILocation(line: 32, column: 5, scope: !49)
!52 = !DILocation(line: 33, column: 18, scope: !13)
!53 = !DILocation(line: 33, column: 25, scope: !13)
!54 = !DILocation(line: 33, column: 23, scope: !13)
!55 = !DILocation(line: 33, column: 29, scope: !13)
!56 = !DILocation(line: 33, column: 34, scope: !13)
!57 = !DILocation(line: 33, column: 33, scope: !13)
!58 = !DILocation(line: 33, column: 27, scope: !13)
!59 = !DILocation(line: 33, column: 38, scope: !13)
!60 = !DILocation(line: 33, column: 36, scope: !13)
!61 = !DILocation(line: 33, column: 2, scope: !13)
!62 = !DILocation(line: 34, column: 2, scope: !13)
