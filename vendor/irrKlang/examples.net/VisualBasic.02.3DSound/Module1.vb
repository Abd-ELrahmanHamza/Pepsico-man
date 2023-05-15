Imports IrrKlang

Module Module1

    Sub Main()

        ' start up the engine
        Dim engine As New ISoundEngine

        ' Now play some sound stream as music in 3d space, looped.
        ' We play it at position (0,0,0) in 3d space

        Dim music As ISound = engine.Play3D("../../media/ophelia.mp3", 0, 0, 0, True)

        ' the following step isn't necessary, but to adjust the distance where
        ' the 3D sound can be heard, we set some nicer minimum distance
        ' (the default min distance is 1, for a small object). The minimum
        ' distance simply is the distance in which the sound gets played
        ' at maximum volume.

        If Not (music Is Nothing) Then
            music.MinDistance = 5.0F
        End If

        ' Print some help text and start the display loop

        Console.Out.Write(ControlChars.CrLf & "Playing streamed sound in 3D.")
        Console.Out.WriteLine(ControlChars.CrLf & "Press ESCAPE to quit, any other key to play sound at random position." & ControlChars.CrLf)

        Console.Out.WriteLine("+ = Listener position")
        Console.Out.WriteLine("o = Playing sound")

        Dim rand As Random = New Random
        Dim radius As Single = 5
        Dim posOnCircle As Single = 5

        While (True) '  endless loop until user exits

            ' Each step we calculate the position of the 3D music.
            ' For this example, we let the
            ' music position rotate on a circle:

            posOnCircle += 0.0399999991F
            Dim pos3D As Vector3D = New Vector3D(radius * Math.Cos(posOnCircle), 0, _
                         radius * Math.Sin(posOnCircle * 0.5F))

            ' After we know the positions, we need to let irrKlang know about the
            ' listener position (always position (0,0,0), facing forward in this example)
            ' and let irrKlang know about our calculated 3D music position

            engine.SetListenerPosition(0, 0, 0, 0, 0, 1)

            If Not (music Is Nothing) Then
                music.Position = pos3D
            End If

            ' Now print the position of the sound in a nice way to the console
            ' and also print the play position

            Dim stringForDisplay As String = "          +         "
            Dim charpos As Integer = Math.Floor((CSng(pos3D.X + radius) / radius * 10.0F))
            If (charpos >= 0 And charpos < 20) Then
                stringForDisplay = stringForDisplay.Remove(charpos, 1)
                stringForDisplay = stringForDisplay.Insert(charpos, "o")
            End If

            Dim playPos As Integer
            If Not (music Is Nothing) Then
                playPos = Integer.Parse(music.PlayPosition.ToString) ' how to convert UInt32 to Integer in visual basic?
            End If

            Dim output As String = ControlChars.Cr & String.Format("x:({0})   3dpos: {1:f} {2:f} {3:f}, playpos:{4}:{5:00}    ", _
             stringForDisplay, pos3D.X, pos3D.Y, pos3D.Z, _
             playPos \ 60000, (playPos Mod 60000) / 1000)

            Console.Write(output)

            System.Threading.Thread.Sleep(100)

            ' Handle user input: Every time the user presses a key in the console,
            ' play a random sound or exit the application if he pressed ESCAPE.

            If _kbhit() <> 0 Then
                Dim key As Integer = _getch()

                If (key = 27) Then
                    Return ' user pressed ESCAPE key
                Else

                    ' Play random sound at some random position.

                    Dim pos As Vector3D = New Vector3D((rand.NextDouble() Mod radius * 2.0F) - radius, 0, 0)

                    Dim filename As String

                    If (rand.Next() Mod 2 <> 0) Then
                        filename = "../../media/bell.wav"
                    Else
                        filename = "../../media/explosion.wav"
                    End If

                    engine.Play3D(filename, pos.X, pos.Y, pos.Z)

                    Console.WriteLine(ControlChars.CrLf & "playing {0} at {1:f} {2:f} {3:f}", _
                        filename, pos.X, pos.Y, pos.Z)
                End If

            End If

        End While

    End Sub

    ' some simple function for reading keys from the console
    <System.Runtime.InteropServices.DllImport("msvcrt")> _
    Public Function _getch() As Integer
    End Function
    <System.Runtime.InteropServices.DllImport("msvcrt")> _
    Public Function _kbhit() As Integer
    End Function

End Module
