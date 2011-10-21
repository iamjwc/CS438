(define folder    "")
(define extension ".in")

; Function takes a filename as a parameter and returns a board list
(define get-board
  (lambda (filename)
    (let ((f (open-input-file (string-append folder filename extension))))
      (let ((r read))  
        (list
          (list 'n    'n    (r f) (r f) (r f) 'n    'n   )
          (list 'n    'n    (r f) (r f) (r f) 'n    'n   )
          (list (r f) (r f) (r f) (r f) (r f) (r f) (r f))
          (list (r f) (r f) (r f) (r f) (r f) (r f) (r f))
          (list (r f) (r f) (r f) (r f) (r f) (r f) (r f))
          (list 'n    'n    (r f) (r f) (r f) 'n    'n   )
          (list 'n    'n    (r f) (r f) (r f) 'n    'n   )
        )
      )
    )
  )
)

; Function gets the item of a list at a specified depth
(define get-index
  (lambda (i l)
    ; If i == 0, return the first item in l, otherwise, recursively call
    ; on the cdr of the list
    (if (eq? i 0)
      (car  l)
      (get-index (- i 1) (cdr l))
    )
  )
)

; Wrapper function for recursive-replace-index
(define replace-index
  (lambda (i v l)
    (recursive-replace-index i v '() l)
  )
)

; Function recursively calls until it finds the index desired
; and replaces that item with variable i.
(define recursive-replace-index
  (lambda (i v firstl secondl)
    (if (eq? i 0)
      (append firstl (list v) (cdr secondl))
      (recursive-replace-index
        (- i 1)
        v
        (append firstl (list (car secondl)))
        (cdr secondl)
      )
    )
  )
)

(define replace-position
  (lambda (v pos board)
    (get-index (col pos) board)
  )
)

; Function that takes a position and returns its contents
; If the position passed is not valid, returns 'n
(define at-position
  (lambda (pos board)
    (if (is-on-board? pos)
      (get-index (col pos) (get-index (row pos) board))
      'n
    )
  )
)

; Function returns the row from a position list
(define row
  (lambda (pos)
    (car pos)
  )
)

; Function returns the column from a position list
(define col
  (lambda (pos)
    (cadr pos)
  )
)

; Function returns whether a position is currently on the board
(define is-on-board?
  (lambda (pos)
    (or
      ; If (col > 0 and col < 2) and (row > 1 and row < 4)
      (and
        (and
          (> (col pos) 0)
          (< (col pos) 2)
        )
        (and
          (> (row pos) 1)
          (< (row pos) 4)
        )
      )
      (or
        ; If (col > 1 and col < 5) and row < 7
        (and
          (and
            (> (col pos) 1)
            (< (col pos) 5)
          ) 
          (< (row pos) 7)
        )
        ; If (col > 4 and col < 7) and (row > 1 and row < 4)
        (and
          (and
            (> (col pos) 4)
            (< (col pos) 7)
          )
          (and
            (> (row pos) 1)
            (< (row pos) 4)
          )
        )
      )
    )
  )
)

; Function that determines if a position currently has no peg
(define is-open?
  (lambda (pos board)
    (and
      (is-on-board? pos)
      (eq? 0 (at-position pos board))
    )
  )
)

; Function that determines if a position currently has a peg
(define is-closed?
  (lambda (pos board)
    (and
      (is-on-board? pos)
      (not (is-open? pos board))
    )
  )
)

; Function moves X spaces to the DIRECTION from POS
(define x-to-the
  (lambda (x direction pos)
  ;  (if (eq? direction 'left) 
  ;    (list (row pos) (- (col pos) x))
  ;    (if (eq? direction 'right)
  ;      (list (row pos) (+ (col pos) x))
  ;      (if (eq? direction 'up)
  ;        (list (- (row pos) x) (col pos))
  ;        (if (eq? direction 'down)
  ;          (list (+ (row pos) x) (col pos))
  ;        )
  ;      )
  ;    )
  ;  )
    (cond
      ((eq? direction 'left)  (list (- (row pos) x) (col pos)))
      ((eq? direction 'right) (list (+ (row pos) x) (col pos)))
      ((eq? direction 'up)    (list (row pos) (- (col pos) x)))
      ((eq? direction 'down)  (list (row pos) (+ (col pos) x)))
    )
  )
)

; Function that determines if a jump is valid from a position
(define can-jump?
  (lambda (direction pos board)
    (and
      (is-closed? pos board)
      (and
        (is-closed? (x-to-the 1 direction pos) board)
        (is-open?   (x-to-the 2 direction pos) board)
      )
    )
  )
)

(define jump
  (lambda (direction pos board)
    (if (can-jump? direction pos board)
      board
      ; CRAP!! How do you replace a certain part of a list.
      ; I guess ill have to do it manually.
    )
  )
)

(define jump-left
  (lambda (pos board)
    (display board)
  )
)
