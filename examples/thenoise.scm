(color 255 255 255 255)
(loop 64 (lambda (i)
    (color (- 255 (* i 8)) 55 55 255)
    (teleport 0 i)
    (loop 8 (lambda (ii)
        (define v (* 8 (+ i ii 1)))
        (down-right v)
        (up-right (floor (/ v 2)))
    ))
))