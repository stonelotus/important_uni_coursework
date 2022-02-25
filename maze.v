`timescale 1ns / 1ps
//////////////////////////////////////////////////////////////////////////////////
// Company: 
// Engineer: 
// 
// Create Date:    01:39:34 12/01/2021 
// Design Name: 
// Module Name:    maze 
// Project Name: 
// Target Devices: 
// Tool versions: 
// Description: 
//
// Dependencies: 
//
// Revision: 
// Revision 0.01 - File Created
// Additional Comments: 
//
//////////////////////////////////////////////////////////////////////////////////
module maze(
	input clk,
	input [6 - 1:0]  starting_col, starting_row, 	// indicii punctului de start
	input maze_in, 											// ofera informa?ii despre punctul de coordonate [row, col]
	
	output reg [6 - 1:0] row, col,	 					// selecteaza un rând si o coloana din labirint
	output reg  maze_oe,			// output enable (activeaza citirea din labirint la rândul ?i coloana date) - semnal sincron	
	output reg  maze_we, 		// write enable (activeaza scrierea în labirint la rândul ?i coloana date) - semnal sincron
	output reg  done
);		 						    	// ie?irea din labirint a fost gasita; semnalul ramane activ 


// Needed regs 
reg [7:0] state;          // Current state 
reg [7:0] next_state;     // Next state
reg [7:0] new_row,new_col;// Aux variables for row/col - needed to go back to current_position
reg [7:0] direction;	     // Going (facing) direction
	
// Going (facing) directions 
`define direction_right 1  
`define direction_down 	2
`define direction_left 	3
`define direction_up 	4

// The states
`define entry_state 0		
`define movement_state 1
`define choose_direction_state 2
`define position_write_state 3
`define finish_state 4

always @(posedge clk) begin
	if(done == 0) begin
		state <= next_state;
	end 
end

always @(*) begin
	done = 0;
	maze_oe = 0;
	maze_we = 0;
	next_state = `entry_state;

	case(state)
		////// The entry state, preparing the first step
		`entry_state: begin		// initial state!
			direction = `direction_right;  
			row = starting_row;	//entry point to be written
			col = starting_col;	//entry point to be written
							
			maze_oe = 1;	// we need to check if the entry point is empty
			next_state = `movement_state;
		end
		
		////////
		/// State to modify position based on following the "right-wall" by direction
		////////
		`movement_state: begin
			if(maze_in) begin 	// maze_in == 1, our relative "right-wall" based on current	
				case(direction)	// direction is not free! Which is good, as we want to follow
					`direction_right: begin row=new_row;  col=new_col+1; end
					`direction_down: 	begin col=new_col;  row=new_row+1; end
					`direction_left: 	begin row=new_row;  col=new_col-1; end
					`direction_up: 	begin col=new_col;  row=new_row-1; end
					default: ;
				endcase
			end else begin	      // maze_in == 0, the incoming (row,col) is free. Means we do not have
				new_row = row;	   // wall in our "right-side" based on our current direction
				new_col = col;		
				direction = direction + 1;	// So here we just try another direction and go forward
				if(direction == 5) begin	// this should never be true, but "just to make sure" 
					direction = `direction_right;				
				end
			end
			
			maze_oe = 1;
			next_state = `position_write_state;
			
		end
		
		////////
		/// State to change direction if going position is not free
		////////
		`position_write_state: begin
			case(maze_in)
				1: begin				
					case(direction) 
						`direction_up:    direction = `direction_down;
						`direction_left:  direction = `direction_right;
						`direction_down:  direction = `direction_up;
						`direction_right: direction = `direction_left;
						default: ;
					endcase
					row = new_row; // if position is not free, change direction and retry 
					col = new_col; // with these 2 lines, we "go back to current position"
					maze_oe = 1;
					new_row = row;
					new_col = col;
					next_state = `choose_direction_state;
				end
				
				0: begin	
					if(!maze_in && (row == 0 || col == 0 || row == 63 || col == 63)) begin		// finish check
						next_state = `finish_state;
					end else begin 	
						new_row = row;					
						new_col = col;  //save current position, in next state row/col will get modified  
						row = new_row;	 // for wall-checking 
						col = new_col;
						
						next_state = `choose_direction_state;
					end
						maze_we = 1;	// current position is free and valid, write current position on matrix
				end
			endcase
		end
		
		
		`choose_direction_state: begin	// apply the wall follower algortihm
			case(direction)					// always followin right side of current direction
				`direction_left:  begin new_row = row; row=row-1;end  // right of left is up
				`direction_up: 	begin new_col = col; col=col+1;end  // right of up is direction_right
				`direction_right: begin new_row = row; row=row+1;end	// right of direction_right is bottom
				`direction_down:  begin new_col = col; col=col-1;end  // right of bottom is left
				default: ;
			endcase
			
			// we save col/row in new_col/new_row and edit col/row
			// then send them to movement_state to verify if col/row - our right-side wall - is free or not
			// if it's free, it is no good, as we need to always have a wall in our "right-side" by direction 
			maze_oe = 1;
			next_state = `movement_state;
		end
		
		`finish_state: begin
			done = 1;
		 end
		
		default: begin
			done = 0;
		end
	endcase
end

endmodule
