from __future__ import print_function
import sys
import time
import math
import random
import numpy as np

def eprint(*args, **kwargs):
    print(*args, file=sys.stderr, **kwargs)

# Hyperparameter
factor = 1.5
max_blank_moves = 20

class Pair():
	def __init__(self, x, y):
		self.x = x
		self.y = y

	def __str__(self):
		return ("(" + str(self.x) + ", " + str(self.y) + ")")

class Game():
	def __init__(self, id, rows, cols):
		self.id = id
		self.rows = rows
		self.cols = cols
		self.direction = 1 - 2 * self.id;

		self.current_player = 0
		self.current_soldier = [[-1, -1], [-1, -1]]

		self.soldiers = []

		self.moves = []
		self.bombs = []

		self.board = [[0 for j in range(rows)] for i in range(cols)]
		self.soldiers_indices = [[-1 for j in range(rows)] for i in range(cols)]

		self.PlaceSoldiers()
		self.PlaceTownHalls()

	def PlaceSoldiers(self):
		places = ([(self.rows - 1 - i) for i in range(3)], [i for i in range(3)])
		for i in range(self.cols):
			for j in range(3):
				self.board[i][places[i % 2][j]] = math.pow(-1, i % 2)
				if(i % 2 == self.id):
					self.soldiers.append(Pair(x = i, y = places[i % 2][j]));
					self.soldiers_indices[i][places[i % 2][j]] = len(self.soldiers) - 1

	def PlaceTownHalls(self):
		for i in range(self.cols // 2):
			self.board[i * 2 + 1][self.rows - 1] = 2;
			self.board[self.cols - 2 - i * 2][0] = -2;

	def IsInBoard(self, x, y):
		if(x >= 0 and x <= (self.cols - 1) and y >= 0 and y <= (self.rows - 1)):
			return 1
		return 0

	def Sign(self, i):
		if(i > 0):
			return 1
		if(i == 0):
			return 0
		if(i < 0):
			return -1

	def SelectSoldier(self, x, y):
		self.moves = []
		self.bombs = []

		executable = self.Guides(x, y)
		if(executable == 0):
			return 0

		self.current_soldier[self.current_player] = [x, y]

		return 1

	def Guides(self, x, y):
		self.moves = []
		self.bombs = []
		executable = 0;

		b = [1, 1, 1, 0, 0]
		dx = [-1, 0, 1, -1, 1]
		dy = [-1, -1, -1, 0, 0]
		for i in range(len(dx)):
			tx = x + dx[i]
			ty = y + dy[i] * self.direction

			if(not self.IsInBoard(tx, ty) or (self.Sign(self.board[tx][ty]) == self.direction) or ((b[i] == 0) and (self.Sign(self.board[tx][ty]) != -self.direction))):
				continue

			self.moves.append(Pair(x = tx, y = ty))
			executable = 1

		check = 0
		adjx = [-1, -1, 0, 1, 1]
		adjy = [0, -1, -1, -1, 0]
		for i in range(len(adjx)):
			tx = x + adjx[i]
			ty = y + adjy[i] * self.direction;

			if(self.IsInBoard(tx, ty) and self.board[tx][ty] == math.pow(-1, 1 - self.id)):
				check = 1
				break

		dx = [-2, 0, 2]
		dy = [2, 2, 2]
		if(check):
			for i in range(len(dx)):
				tx = x + dx[i]
				ty = y + dy[i] * self.direction

				if(not self.IsInBoard(tx, ty) or self.Sign(self.board[tx][ty]) == self.direction):
					continue

				self.moves.append(Pair(x = tx, y = ty))

				executable = 1

		dx = [[[0, 0, 0, 0], [0, 0, 0, 0], [0, 0, 0, 0]], [[-3, -2, 4, 5], [-4, -3, 3, 4], [-5, -4, 2, 3]], [[-3, -2, 4, 5], [-4, -3, 3, 4], [-5, -4, 2, 3]], [[-3, -2, 4, 5], [-4, -3, 3, 4], [-5, -4, 2, 3]]]
		dy = [[[-3, -2, 4, 5], [-4, -3, 3, 4], [-5, -4, 2, 3]], [[-3, -2, 4, 5], [-4, -3, 3, 4], [-5, -4, 2, 3]], [[0, 0, 0, 0], [0, 0, 0, 0], [0, 0, 0, 0]], [[3, 2, -4, -5], [4, 3, -3, -4], [5, 4, -2, -3]]]
		valid_dx = [[[0, 0], [0, 0], [0, 0]], [[-1, 3], [-2, 2], [-3, 1]], [[-1, 3], [-2, 2], [-3, 1]], [[-1, 3], [-2, 2], [-3, 1]]]
		valid_dy = [[[-1, 3], [-2, 2], [-3, 1]], [[-1, 3], [-2, 2], [-3, 1]], [[0, 0], [0, 0], [0, 0]], [[1, -3], [2, -2], [3, -1]]]
		cdx = [[[0], [], [0]], [[3], [], [-3]], [[3], [], [-3]], [[3], [], [-3]]]
		cdy = [[[3], [], [-3]], [[3], [], [-3]], [[0], [], [0]], [[-3], [], [3]]]
		soldierx = [[[0, 0], [0, 0], [0, 0]], [[1, 2], [-1, 1], [-2, -1]], [[1, 2], [-1, 1], [-2, -1]], [[1, 2], [-1, 1], [-2, -1]]]
		soldiery = [[[1, 2], [-1, 1], [-2, -1]], [[1, 2], [-1, 1], [-2, -1]], [[0, 0], [0, 0], [0, 0]], [[-1, -2], [1, -1], [2, 1]]]

		for i in range(len(dx)):
			for j in range(len(dx[0])):
				check = 1

				s = ([x + valid_dx[i][j][0], y + valid_dy[i][j][0] * self.direction], [x + valid_dx[i][j][1], y + valid_dy[i][j][1] * self.direction])
				if((self.IsInBoard(s[0][0], s[0][1]) and (self.board[s[0][0]][s[0][1]] == 0)) or (self.IsInBoard(s[1][0], s[1][1]) and (self.board[s[1][0]][s[1][1]] == 0))):
					for k in range(len(soldierx[0][0])):
						tx = x + soldierx[i][j][k];
						ty = y + soldiery[i][j][k] * self.direction;

						if(not self.IsInBoard(tx, ty) or self.board[tx][ty] != self.direction):
							check = 0

					if(check):
						for k in range(len(dx[0][0])):
							if((self.IsInBoard(s[k // 2][0], s[k // 2][1]) and (self.board[s[k // 2][0]][s[k // 2][1]] == 0))):
								tx = x + dx[i][j][k]
								ty = y + dy[i][j][k] * self.direction

								if(self.IsInBoard(tx, ty) and self.Sign(self.board[tx][ty]) != self.direction):
									self.bombs.append(Pair(x = tx, y = ty))
									executable = 1

						for k in range(len(cdx[i][j])):
							tx = x + cdx[i][j][k]
							ty = y + cdy[i][j][k] * self.direction;

							if(not self.IsInBoard(tx, ty) or self.board[tx][ty] != 0):
								continue

							self.moves.append(Pair(x = tx, y = ty))
							executable = 1
		# eprint ("For soldier at " + str(x) + ", " + str(y) + ": " + str([str(x) for x in self.moves]))
		return executable

	def MoveSoldier(self, x, y):
		self.board[self.current_soldier[self.current_player][0]][self.current_soldier[self.current_player][1]] = 0

		if(self.current_player == self.id):
			index = self.soldiers_indices[self.current_soldier[self.current_player][0]][self.current_soldier[self.current_player][1]]
			self.soldiers_indices[self.current_soldier[self.current_player][0]][self.current_soldier[self.current_player][1]] = -1
			self.soldiers_indices[x][y] = index
			self.soldiers[index] = Pair(x = x, y = y)
		else:
			if(self.board[x][y] == self.direction):
				index = self.soldiers_indices[x][y]
				self.soldiers_indices[x][y] = -1
				self.soldiers[index] = Pair(x = -1, y = -1)

		self.board[x][y] = math.pow(-1, self.current_player)
		self.current_player = 1 - self.current_player

	def ThrowBomb(self, x, y):
		if(self.current_player == 1 - self.id):
			if(self.board[x][y] == self.direction):
				index = self.soldiers_indices[x][y]
				self.soldiers_indices[x][y] = -1
				self.soldiers[index] = Pair(x = -1, y = -1)

		self.board[x][y] = 0
		self.current_player = 1 - self.current_player


	def execute_sequence(self, sequence):
		move = []
		for index, j in enumerate(sequence):
			if(index % 3 == 2):
				move += [j]
				self.execute_move(' '.join(move))
				move = []
			else:
				move += [j]

	def execute_move(self, command) :
		sequence = command.split()

		if(len(sequence) > 3):
			return self.execute_sequence(sequence)

		type = sequence[0]
		x = int(sequence[1])
		y = int(sequence[2])

		if(type == 'S'):
			status = self.SelectSoldier(x, y)
		elif(type == 'M'):
			status = self.MoveSoldier(x, y)
		elif(type == 'B'):
			status = self.ThrowBomb(x, y)

		return status

class NeutralPlayer:
	def __init__(self):
		data = sys.stdin.readline().strip().split()
		self.player = int(data[0]) - 1
		self.rows = int(data[1])
		self.cols = int(data[2])
		self.time_left = int(data[3])
		self.num_blank_moves = 0
		self.game = Game(id = self.player, rows = self.rows, cols = self.cols)
		self.state = 0
		self.play()

	# Check if the given soldier is being attacked by the enemy army
	def GetSeverity(self, idx, idy):
		x, y = idx, idy
		direction = self.game.direction
		severity = 0

		# Check if soldier can be attacked by any immediate enemy
		enemies = [(-2, 2), (-1, -1), (-1, 0), (0, -1), (0, 2), (1, -1), (1, 0), (2, 2)]
		for enemy in enemies:
			ex, ey = x + enemy[0], y + direction*enemy[1]
			# eprint ("Checking for coordinate " + str(ex) + ", " + str(ey))
			if self.game.IsInBoard(ex, ey) and (self.game.board[ex][ey] == math.pow(-1, 1 - self.player)):
				# Enemy can attack - move from current position.
				# eprint ("Found attack")
				severity += 1

		# Check if soldier can be attacked by any cannon
		cannons = [(-1, 0), (-1, -1), (0, -1), (1, -1), (1, 0), (1, 1), (0, 1), (-1, 1)]  # The directions from where the cannons can attack
		blanks = [2, 3]
		for cannon in cannons:
			for blank in blanks:
				# First check if cannon attack is even possible from this direction
				cx, cy = x + cannon[0]*(blank - 1), y + cannon[1]*(blank - 1)
				# eprint ("Checking for cannon at " + str(x + cannon[0]*blank) + ", " + str(y + cannon[1]*blank))
				if self.game.IsInBoard(cx, cy) and self.game.board[cx][cy] == 0:
					# If cannon attack possible, check if cannon exists or not
					# eprint ("Attack possible - checking if cannon exists")
					flag = True
					for i in range(3):
						ex, ey = x + cannon[0]*(i + blank), y + cannon[1]*(i + blank)
						if self.game.IsInBoard(ex, ey) and self.game.board[ex][ey] == math.pow(-1, 1 - self.player):
							continue
						flag = False
						break
					if flag:
						# Cannon Present
						# eprint ("Found attack")
						severity += 1

		# Assign higher severity to soldiers who are nearer to Townhalls
		if (self.player == 1 and y < 3) or (self.player == 0 and y > 6):
			severity = severity * factor

		return severity

	def SelectSoldierAndMove(self):
		severeSoldier = 0
		maxSeverity = 0
		sx, sy = -1, -1
		mx, my = -1, -1
		moveOrBomb = 0
		flag = 0

		# Choose a soldier that can immediately attack a townhall
		for j in range(len(self.game.soldiers)):
			jx, jy = self.game.soldiers[j].x, self.game.soldiers[j].y
			if jx == -1 or jy == -1:
				continue

			self.game.Guides(jx, jy)
			# Check if it can attack a townhall
			for move in self.game.moves:
				token = self.game.board[move.x][move.y]
				if (move.x != -1 and move.y != -1):
					if (self.player == 0 and token == -2) or (self.player == 1 and token == 2):
						sx, sy = jx, jy
						mx, my = move.x, move.y
						moveOrBomb = 0
						flag = 1
						break
			for bomb in self.game.bombs:
				token = self.game.board[bomb.x][bomb.y]
				if (bomb.x != -1 and bomb.y != -1):
					if (self.player == 0 and token == -2) or (self.player == 1 and token == 2):
						sx, sy = jx, jy
						mx, my = bomb.x, bomb.y
						moveOrBomb = 1
						flag = 1
						break

			if (flag == 1):
				break
			else:
				# Move the soldier which can be most attacked by the enemy
				# eprint("Checking soldier at " + str(self.game.soldiers[j].x) + ", " + str(self.game.soldiers[j].y))
				severity = self.GetSeverity(self.game.soldiers[j].x, self.game.soldiers[j].y)
				if maxSeverity < severity:
					if self.game.Guides(jx, jy) != 0:
						maxSeverity = severity
						severeSoldier = j
		
		if flag == 1:
			self.num_blank_moves = 0
			return sx, sy, mx, my, moveOrBomb
		else:
			sx, sy = self.game.soldiers[severeSoldier].x, self.game.soldiers[severeSoldier].y
	
			# Check if the severe soldier has any remaining moves
			if (sx == -1 or sy == -1 or self.game.Guides(sx, sy) == 0):
				for i in range(len(self.game.soldiers)):
					x, y = self.game.soldiers[i].x, self.game.soldiers[i].y
					v = self.game.Guides(x, y)
					if(x != -1 and y != -1 and v != 0):
						sx, sy = x, y
						break

			bestMove = 0
			minSeverity = -1
			attackMove = (-1, -1)

			self.game.Guides(sx, sy)
			if (len(self.game.moves) != 0):
				for i in range(len(self.game.moves)):
					# If possible, choose a safe move
					move = self.game.moves[i]
					if (move.x != -1 and move.y != -1):
						severity = self.GetSeverity(move.x, move.y)
						if (minSeverity > severity or minSeverity == -1):
							minSeverity = severity
							bestMove = i
						if (self.game.board[move.x][move.y] == math.pow(-1, 1 - self.player)):
							# Save for later
							attackMove = (move.x, move.y)

			# eprint ("Bestmove: " + str(self.game.moves[bestMove]))
			if minSeverity != 0:
				if (len(self.game.bombs) != 0):
					# Choose a bomb shot
					moveOrBomb = 1
					mx, my = self.game.bombs[0].x, self.game.bombs[0].y
					attackCannon = 0
					for i in range(len(self.game.bombs)):
						bx, by = self.game.bombs[i].x, self.game.bombs[i].y
						if (bx != -1 and by != -1 and self.game.board[bx][by] == math.pow(-1, 1 - self.player)):
							attackCannon = 1
							mx, my = bx, by
							break
					if attackCannon == 0:
						self.num_blank_moves += 1
				else:
					# Choose a move that can attack some enemy
					self.num_blank_moves = 0
					if attackMove != (-1, -1):
						moveOrBomb = 0
						mx, my = attackMove[0], attackMove[1]
					else:
						moveOrBomb = 0
						mx, my = self.game.moves[bestMove].x, self.game.moves[bestMove].y
			else:
				moveOrBomb = 0
				self.num_blank_moves += 1
				mx, my = self.game.moves[bestMove].x, self.game.moves[bestMove].y

		if self.num_blank_moves >= max_blank_moves:
			# Choose a random move for any soldier
			flag = 0
			for si in range(len(self.game.soldiers)):
				sx, sy = self.game.soldiers[si].x, self.game.soldiers[si].y
				exc = self.game.Guides(sx, sy)
				if (sx != -1 and sy != -1 and exc != 0):
					for mi in range(len(self.game.moves)):
						mx, my = self.game.moves[mi].x, self.game.moves[mi].y
						if (mx != -1 and my != -1):
							moveOrBomb = 0
							flag = 1
							break
					if flag == 1:
						break

		return sx, sy, mx, my, moveOrBomb

	def play_sequence(self, sequence):
		moves = ' '.join(sequence) + '\n'
		sys.stdout.write(moves)
		sys.stdout.flush()

	def play(self):
		if(self.player == 1):
			move = sys.stdin.readline().strip()
			self.game.execute_move(move)

		num_moves = 0
		while(1):
			if num_moves == 0:
				# Deterministic Fort Development Strategy
				num_moves += 1
				if self.player == 0:
					sequence = ['S 2 7', 'M 1 6']
				else:
					sequence = ['S 3 0', 'M 2 1']
			elif num_moves == 1:
				num_moves += 1
				if self.player == 0:
					sequence = ['S 4 7', 'M 5 6']
				else:
					sequence = ['S 5 0', 'M 6 1']
			else:
				sequence = []
				sx, sy, mx, my, moveOrBomb = self.SelectSoldierAndMove()
				sequence.append('S {x} {y}'.format(x = sx, y = sy))
				if moveOrBomb == 0:
					sequence.append('M {x} {y}'.format(x = mx, y = my))
				else:
					sequence.append('B {x} {y}'.format(x = mx, y = my))

			for move in sequence:
				self.game.execute_move(move)

			self.state = 0
			self.play_sequence(sequence)

			move = sys.stdin.readline().strip()
			self.game.execute_move(move)

random.seed(0)
NeutralPlayer()
