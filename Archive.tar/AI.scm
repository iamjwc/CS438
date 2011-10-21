(define folder    "")
(define extension ".in")
(define solved '((n n 0 0 0 n n) (n n 0 0 0 n n) (0 0 0 0 0 0 0) (0 0 0 1 0 0 0) (0 0 0 0 0 0 0) (n n 0 0 0 n n) (n n 0 0 0 n n)))

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

; Function that replaces a value at a given index in list l
(define replace-index
  (lambda (index value l)
    (recursive-replace-index index value '() l)
  )
)

; Function that does the replacement recursively
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

; Function that takes a position and returns its contents
; If the position passed is not valid, returns nothing
(define at-position
  (lambda (pos board)
    (if (is-on-board? pos)
      (get-index
        (col pos)
        (get-index
          (row pos)
          board
        )
      )
    )
  )
)

; Function replaces whatever is at pos with the value
; on the board
(define replace-position
  (lambda (pos value board)
    (replace-index
      (row pos)
      (replace-index
        (col pos)
        value
        (get-index (row pos) board)
      )
      board
    )
  )
)

; Function returns the col from a position list
(define col
  (lambda (pos)
    (cadr pos)
  )
)

; Function returns the rowumn from a position list
(define row
  (lambda (pos)
    (car pos)
  )
)

; Function returns whether a position is currently on the board
(define is-on-board?
  (lambda (pos)
    (or
      ; If (row >= 0 and row <= 1) and (col >= 2 and col <= 4)
      (and
        (and
          (>= (row pos) 0)
          (<= (row pos) 1)
        )
        (and
          (>= (col pos) 2)
          (<= (col pos) 4)
        )
      )
      (or
        ; If (row >= 0 and row <= 6) and (col >= 0 and col <= 6)
        (and
          (and
            (>= (row pos) 0)
            (<= (row pos) 6)
          )
          (and
            (>= (col pos) 0)
            (<= (col pos) 6)
          )
        )
        ; If (row >= 5 and row <= 6) and (col >= 2 and col <= 4)
        (and
          (and
            (>= (row pos) 5)
            (<= (row pos) 6)
          )
          (and
            (>= (col pos) 2)
            (<= (col pos) 4)
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
    (cond
      ((eq? direction 'left)  (list (row pos) (- (col pos) x)))
      ((eq? direction 'right) (list (row pos) (+ (col pos) x)))
      ((eq? direction 'up)    (list (- (row pos) x) (col pos)))
      ((eq? direction 'down)  (list (+ (row pos) x) (col pos)))
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

; Function that performs the jump and returns the board
(define jump
  (lambda (direction pos board)
    (if (can-jump? direction pos board)
      (replace-position
        (x-to-the 2 direction pos)
        1
        (replace-position
          (x-to-the 1 direction pos)
          0
          (replace-position pos 0 board)
        )
      )
      '()
    )
  )
)

; Function that combines the sublists of a pegboard into one list
(define combine-sublists
  (lambda (l)
    (if (not (empty? l))
       (append (car l) (combine-sublists (cdr l)))
      '()
    )
  )
)

; Gets a list of length X if 3, gets (0 1 2)
(define list-of-length
  (lambda (len)
    (if (not (eq? 0 len))
      (append
        (list-of-length
          (- len 1)
        )
        (list (- len 1))
      )
      '()
    )
  )
)

(define remove-empty-lists
  (lambda (l)
    (recursive-remove-empty-lists '() l)
  )
)

(define recursive-remove-empty-lists
  (lambda (front back)
    (if (empty? back)
      front
      (if (empty? (car back))
        (recursive-remove-empty-lists front (cdr back))
        (recursive-remove-empty-lists
          (append front (list (car back)))
          (cdr back)
        )
      )
    )
  )
)

(define do-jumps
  (lambda (pos board)
    (remove-empty-lists
      (map
        (lambda (direction)
          (jump direction pos board)
        )
        '(up right down left)
      )
    )
  )
)

; Function that calls a function X number of times
(define times
  (lambda (n f)
    (map ;for-each
      f
      (list-of-length n)
    )
  )
)

(define combine-sublists
  (lambda (l)
    (if (empty? l)
      '()
      (append (car l) (combine-sublists (cdr l)))
    )
  )
)

(define _find-all-jumps
  (lambda (board)
    (times (length board)
      (lambda (i)
        (times (length (car board))
          (lambda (j)
            (if (eq? 1 (at-position (list i j) board))
              (do-jumps (list i j) board)
              '()
            )
          )
        )
      )
    )
  )
)

(define find-all-jumps
  (lambda (board)
    (combine-sublists(combine-sublists
      (_find-all-jumps board)
    ))
  )
)

(define solved?
  (lambda (b)
    (equal? b solved)
  )
)

(define dfs
  (lambda (ancestors children)
    ; If there are no children
    (if (empty? children)
      ; If the last ancestor is solved, return ancestors
      (if (solved? (car (reverse ancestors)))
        ancestors
        ; Else return empty list (no solution)
        '()
      )
      ; Else call dfs on each child
      (remove-empty-lists
        (map
          (lambda (child)
            (dfs (append ancestors (list child)) (find-all-jumps child))
          )
          children
        )
      )
    )
  )
)

;(define count-pegs-in-row
;  (lambda (row)
;    (do
;      (
;        (col 0 (+ col 1))
;        (pegs 0)
;      )
;      (
;        (= col 7)
;        pegs
;      )
;      
;      (if (eq? (get-index col row) 1)
;        (set! pegs (+ pegs 1))
;      )
;    )
;  )
;)
;
;(define count-pegs
;  (lambda (board)
;    (do
;      (
;        (row 0 (+ row 1))
;        (pegs 0)
;      )
;
;      (
;        (= row 7)
;        pegs
;      )
;
;      (set! pegs (+ pegs (count-pegs-in-row (get-index row board))))
;    )
;  )
;)
;
;(define remove-dfs-trash
;  (lambda (path)
;    (let ((new-path '()) (temp (car path)))
;      (for-each
;        (lambda (board)
;          (if (not (eq? (count-pegs temp) (count-pegs board)))
;            (set! new-path (append new-path (list temp)))
;          )
;          (set! temp board)
;        )
;        (cdr path)
;      )
;      new-path
;    )
;  )
;)

(define ndfs
  (lambda (board)
    ; Else, process all of the nodes and return list of boards
    (do
      ; Init
      (
        (children '()) ;find-all-jumps board) (cdr children))
        (stack (list board))
      )

      ; Iterative test & Return val  
      (
        ; Loop until children are null, or stack is solved
        (or
        ;  (or
        ;    (empty? children)
           (empty? stack)
        ;  )
          (solved? (car stack))
        )

        ; If the car of the stack is solved, return the stack, 
        ; otherwise return empty list
        ;(if (solved? (car stack))
          (reverse stack)
        ;  '()
        ;)
      )
      

      (let ((child-nodes (find-all-jumps (car stack))))

        (if (empty? children)
          (set! children child-nodes)
          (set! children (append child-nodes (cdr children)))
        )
        
        (if (empty? child-nodes)
          ; If there are no child nodes from the current node, pop
          ; off of stack, and push the next child onto the stack
          (set! stack (cons (car children) (cdr stack)))

          ; Else, push the first child onto the stack
          (set! stack (cons (car children) stack))
        )
      )


        ;(display "Stack size: ")(display (length stack))(newline)
        ;(display "Child nodes: ")(display (length child-nodes))(newline)
        ;(display stack)(newline)(read-char)
      

    )
  )
)



(define a (get-board "plus"))
(define
  almost
  '((n n 0 0 0 n n)
    (n n 0 0 0 n n)
    (0 0 0 0 0 0 0)
    (0 0 0 0 0 0 0)
    (0 0 0 1 0 0 0)
    (n n 0 1 0 n n)
    (n n 0 0 0 n n))
)

(define
  almost2
  '((n n 0 0 0 n n)
    (n n 0 0 0 n n)
    (0 0 0 1 0 0 0)
    (0 0 0 1 0 0 0)
    (0 0 0 0 0 0 0)
    (n n 0 1 0 n n)
    (n n 0 0 0 n n))
)
